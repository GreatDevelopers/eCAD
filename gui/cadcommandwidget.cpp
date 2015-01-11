/**
 * cadcommandwidget.cpp
 *
 * Copyright (C) 2014 GreatDevelopers
 *
 * eCAD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eCAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "cadcommandwidget.h"

CadCommandWidget::CadCommandWidget()
{
    // widgets
    QLineEdit *lEdit = new QLineEdit;
    QTextEdit *tEdit = new QTextEdit;
    QWidget *w = new QWidget;

    // labels
    QLabel *lLabel = new QLabel("Enter Command:");
    QLabel *tLabel = new QLabel("Command History:");

    // layouts
    QHBoxLayout *hBox = new QHBoxLayout;
    QVBoxLayout *vBox1 = new QVBoxLayout;
    QVBoxLayout *vBox2 = new QVBoxLayout;

    // setting properties
    tEdit->setReadOnly(true);

    vBox1->addWidget(lLabel);
    vBox1->addWidget(lEdit);
    vBox2->addWidget(tLabel);
    vBox2->addWidget(tEdit);
    hBox->addLayout(vBox1);
    hBox->addLayout(vBox2);

    w->setLayout(hBox);
    setWidget(w);
}
