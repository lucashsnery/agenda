#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QDialog>
#include <QLabel>
#include <memory>
#include "Calendar.h"
#include "JsonPersistence.h"
#include "User.h"
#include "Time_x.h"

namespace agenda {

// ---------------- CreateEventDialog ----------------
class CreateEventDialog : public QDialog {
public:
    explicit CreateEventDialog(QWidget* parent = nullptr);
    ~CreateEventDialog() override = default;

    std::string title() const;
    std::string date() const;
    Time_x start_time() const;
    Time_x end_time() const;
    std::vector<std::string> tags() const;
    int recurrence_interval() const;

private:
    QLineEdit* m_title;
    QDateEdit* m_date;
    QLineEdit* m_start;
    QLineEdit* m_end;
    QLineEdit* m_tags;
    QLineEdit* m_recurrence;
    QPushButton* m_ok;
    QPushButton* m_cancel;
};

// ---------------- GUI ----------------
class GUI : public QMainWindow {
public:
    explicit GUI(QWidget* parent = nullptr);
    ~GUI() override = default;

    void load_events_table();

    void on_create_event();
    void on_remove_event();
    void on_filter_tag();
    void on_filter_date();
    void on_save();
    void on_load();

private:
    Calendar m_calendar;
    std::unique_ptr<IPersistence> m_persistence;

    QLabel* m_userLabel;

    QTableWidget* m_table;
    QPushButton* m_createBtn;
    QPushButton* m_removeBtn;
    QLineEdit* m_filterTagEdit;
    QPushButton* m_filterTagBtn;
    QDateEdit* m_filterDateEdit;
    QPushButton* m_filterDateBtn;
    QPushButton* m_saveBtn;
    QPushButton* m_loadBtn;

    void ask_user();
};

} // namespace agenda