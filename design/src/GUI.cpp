#include "GUI.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QLabel>
#include <QUuid>
#include <sstream>
#include <iomanip>
#include "Event.h"
#include "RecurrenceRule.h"
#include "JsonPersistence.h"

namespace agenda {

// ---------------- CreateEventDialog ----------------
CreateEventDialog::CreateEventDialog(QWidget* parent)
    : QDialog(parent)
{
    m_title = new QLineEdit(this);
    m_date = new QDateEdit(QDate::currentDate(), this);
    m_start = new QLineEdit("HH:MM", this);
    m_end = new QLineEdit("HH:MM", this);
    m_tags = new QLineEdit(this);
    m_recurrence = new QLineEdit("0", this);

    m_ok = new QPushButton("OK", this);
    m_cancel = new QPushButton("Cancelar", this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_title);
    mainLayout->addWidget(m_date);
    mainLayout->addWidget(m_start);
    mainLayout->addWidget(m_end);
    mainLayout->addWidget(m_tags);
    mainLayout->addWidget(m_recurrence);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_ok);
    btnLayout->addWidget(m_cancel);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);

    connect(m_ok, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancel, &QPushButton::clicked, this, &QDialog::reject);
}

std::string CreateEventDialog::title() const { return m_title->text().toStdString(); }
std::string CreateEventDialog::date() const { return m_date->date().toString("yyyy-MM-dd").toStdString(); }

Time_x CreateEventDialog::start_time() const {
    int h=0,m=0;
    sscanf(m_start->text().toStdString().c_str(), "%d:%d", &h, &m);
    return Time_x(h,m);
}
Time_x CreateEventDialog::end_time() const {
    int h=0,m=0;
    sscanf(m_end->text().toStdString().c_str(), "%d:%d", &h, &m);
    return Time_x(h,m);
}

std::vector<std::string> CreateEventDialog::tags() const {
    std::vector<std::string> res;
    std::istringstream ss(m_tags->text().toStdString());
    std::string t;
    while (std::getline(ss, t, ',')) res.push_back(t);
    return res;
}

int CreateEventDialog::recurrence_interval() const { return m_recurrence->text().toInt(); }

// ---------------- GUI ----------------
GUI::GUI(QWidget* parent)
    : QMainWindow(parent), m_persistence(std::make_unique<JsonPersistence>())
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;

    // Cabeçalho
    m_userLabel = new QLabel("Usuário: (não definido)", this);
    mainLayout->addWidget(m_userLabel);

    // Tabela
    m_table = new QTableWidget(0,5,this);
    QStringList headers = {"ID","Data","Início-Fim","Tags","Título"};
    m_table->setHorizontalHeaderLabels(headers);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(m_table);

    // Botões
    QHBoxLayout* btnLayout = new QHBoxLayout;
    m_createBtn = new QPushButton("Criar", this);
    m_removeBtn = new QPushButton("Remover", this);
    m_filterTagEdit = new QLineEdit(this);
    m_filterTagBtn = new QPushButton("Filtrar Tag", this);
    m_filterDateEdit = new QDateEdit(QDate::currentDate(), this);
    m_filterDateBtn = new QPushButton("Filtrar Data", this);
    m_saveBtn = new QPushButton("Salvar", this);
    m_loadBtn = new QPushButton("Carregar", this);

    btnLayout->addWidget(m_createBtn);
    btnLayout->addWidget(m_removeBtn);
    btnLayout->addWidget(m_filterTagEdit);
    btnLayout->addWidget(m_filterTagBtn);
    btnLayout->addWidget(m_filterDateEdit);
    btnLayout->addWidget(m_filterDateBtn);
    btnLayout->addWidget(m_saveBtn);
    btnLayout->addWidget(m_loadBtn);
    mainLayout->addLayout(btnLayout);

    central->setLayout(mainLayout);
    setCentralWidget(central);

    // Conexões
    connect(m_createBtn, &QPushButton::clicked, this, &GUI::on_create_event);
    connect(m_removeBtn, &QPushButton::clicked, this, &GUI::on_remove_event);
    connect(m_filterTagBtn, &QPushButton::clicked, this, &GUI::on_filter_tag);
    connect(m_filterDateBtn, &QPushButton::clicked, this, &GUI::on_filter_date);
    connect(m_saveBtn, &QPushButton::clicked, this, &GUI::on_save);
    connect(m_loadBtn, &QPushButton::clicked, this, &GUI::on_load);

    // Perguntar usuário
    ask_user();
    load_events_table();
}

// ---------------- ask_user ----------------
void GUI::ask_user() {
    bool ok1, ok2, ok3;
    QString id = QInputDialog::getText(this, "ID", "Digite o ID do usuário:", QLineEdit::Normal, "", &ok1);
    QString username = QInputDialog::getText(this, "Usuário", "Digite o username:", QLineEdit::Normal, "", &ok2);
    QString display = QInputDialog::getText(this, "Nome", "Digite o nome completo:", QLineEdit::Normal, "", &ok3);

    if (ok1 && ok2 && ok3 && !id.isEmpty() && !username.isEmpty() && !display.isEmpty()) {
        User user(id.toStdString(), username.toStdString(), display.toStdString());
        m_calendar.set_owner(user);
        m_userLabel->setText("Usuário: " + display);
    } else {
        m_userLabel->setText("Usuário: (anônimo)");
    }
}

// ---------------- Funções auxiliares ----------------

void GUI::load_events_table() {
    auto events = m_calendar.all_events();

    // Ordenar por data e hora de início
    std::sort(events.begin(), events.end(), [](const auto& a, const auto& b){
        if (a->date() != b->date()) return a->date() < b->date();
        return a->start_time() < b->start_time();
    });

    m_table->setRowCount(events.size());

    for (int r = 0; r < events.size(); ++r) {
        auto e = events[r];
        m_table->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(e->id())));
        m_table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(e->date())));

        // Formatar horário de início e fim
        int start_h = e->start_time().hour;
        int start_m = e->start_time().minute;
        int end_h = e->end_time().hour;
        int end_m = e->end_time().minute;

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << start_h << ":" 
           << std::setw(2) << std::setfill('0') << start_m << "-"
           << std::setw(2) << std::setfill('0') << (end_h%24) << ":"
           << std::setw(2) << std::setfill('0') << end_m;

        if (e->ends_next_day()) ss << " (dia seguinte)";
        m_table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(ss.str())));

        // Tags
        std::ostringstream tss;
        for (const auto& tag : e->tags()) tss << tag << ",";
        m_table->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(tss.str())));

        // Título
        m_table->setItem(r, 4, new QTableWidgetItem(QString::fromStdString(e->title())));
    }
}

// ---------------- Slots ----------------

void GUI::on_create_event() {
    CreateEventDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) return;

    try {
        auto ev = std::make_unique<Event>(
            QUuid::createUuid().toString().toStdString(),
            dlg.title(),
            dlg.date(),
            dlg.start_time(),
            dlg.end_time()
        );

        // Ajustar fim caso passe da meia-noite
        if (ev->end_time() < ev->start_time()) {
            Time_x adjusted_end = ev->end_time();
            adjusted_end.hour += 24;
            ev->set_time(ev->start_time(), adjusted_end);
        }

        // Adicionar tags
        for (const auto& tag : dlg.tags()) ev->add_tag(tag);

        // Configurar recorrência se necessário
        if (dlg.recurrence_interval() > 0) {
            auto rule = std::make_shared<agenda::RecurrenceRule>(agenda::Frequency::DAILY, dlg.recurrence_interval());
            ev->set_recurrence(rule);
        }

        m_calendar.add_event(std::move(ev));
        load_events_table();

    } catch (const std::exception& ex) {
        QMessageBox::warning(this, "Erro", ex.what());
    }
}

void GUI::on_remove_event() {
    auto row = m_table->currentRow();
    if (row < 0) return;
    auto id = m_table->item(row,0)->text().toStdString();
    if (m_calendar.remove_event(id)) load_events_table();
}

void GUI::on_filter_tag() {
    auto tag = m_filterTagEdit->text().toStdString();
    auto events = m_calendar.find_by_tag(tag);

    m_table->setRowCount(events.size());

    for (int r = 0; r < events.size(); ++r) {
        auto e = events[r];
        m_table->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(e->id())));
        m_table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(e->date())));

        int start_h = e->start_time().hour;
        int start_m = e->start_time().minute;
        int end_h = e->end_time().hour;
        int end_m = e->end_time().minute;

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << start_h << ":" 
           << std::setw(2) << std::setfill('0') << start_m << "-"
           << std::setw(2) << std::setfill('0') << (end_h%24) << ":" 
           << std::setw(2) << std::setfill('0') << end_m;

        if (e->ends_next_day()) ss << " (dia seguinte)";
        m_table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(ss.str())));

        std::ostringstream tss;
        for (const auto& t : e->tags()) tss << t << ",";
        m_table->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(tss.str())));

        m_table->setItem(r, 4, new QTableWidgetItem(QString::fromStdString(e->title())));
    }
}

void GUI::on_filter_date() {
    auto date = m_filterDateEdit->date().toString("yyyy-MM-dd").toStdString();
    auto events = m_calendar.events_on_date(date);

    m_table->setRowCount(events.size());

    for (int r = 0; r < events.size(); ++r) {
        auto e = events[r];
        m_table->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(e->id())));
        m_table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(e->date())));

        int start_h = e->start_time().hour;
        int start_m = e->start_time().minute;
        int end_h = e->end_time().hour;
        int end_m = e->end_time().minute;

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << start_h << ":" 
           << std::setw(2) << std::setfill('0') << start_m << "-" 
           << std::setw(2) << std::setfill('0') << (end_h%24) << ":" 
           << std::setw(2) << std::setfill('0') << end_m;

        if (e->ends_next_day()) ss << " (dia seguinte)";
        m_table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(ss.str())));

        std::ostringstream tss;
        for (const auto& t : e->tags()) tss << t << ",";
        m_table->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(tss.str())));

        m_table->setItem(r, 4, new QTableWidgetItem(QString::fromStdString(e->title())));
    }
}

void GUI::on_save() {
    try {
        m_persistence->save(m_calendar, "agenda.json");
        QMessageBox::information(this, "Sucesso", "Agenda salva com sucesso!");
    } catch(const std::exception& ex) {
        QMessageBox::warning(this,"Erro", ex.what());
    }
}

void GUI::on_load() {
    try {
        auto cal = m_persistence->load("agenda.json");
        m_calendar = std::move(*cal);
        m_userLabel->setText("Usuário: " + QString::fromStdString(m_calendar.owner().display_name()));
        load_events_table();
        QMessageBox::information(this, "Sucesso", "Agenda carregada com sucesso!");
    } catch(const std::exception& ex) {
        QMessageBox::warning(this,"Erro", ex.what());
    }
}

} // namespace agenda