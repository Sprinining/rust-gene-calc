#ifndef GENESETTINGSPANEL_H
#define GENESETTINGSPANEL_H

#include <QWidget>

namespace Ui {
class GeneSettingsPanel;
}

class GeneSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit GeneSettingsPanel(QWidget *parent = nullptr);
    ~GeneSettingsPanel();

private:
    Ui::GeneSettingsPanel *ui;
};

#endif // GENESETTINGSPANEL_H
