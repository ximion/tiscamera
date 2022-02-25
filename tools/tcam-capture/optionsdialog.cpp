/*
 * Copyright 2021 The Imaging Source Europe GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "optionsdialog.h"

#include "ui_optionsdialog.h"



OptionsDialog::OptionsDialog(TcamCaptureConfig& config,
                             const OptionsSettings& /*settings*/,
                             QWidget* parent)
    : QDialog(parent), app_config(config), ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    // these should be in the same order as the enum class
    // that makes get/set easier
    ui->combo_convert_options->addItem("Auto");
    ui->combo_convert_options->addItem("tcamconvert");
    ui->combo_convert_options->addItem("tcamdutils");
    ui->combo_convert_options->addItem("tcamdutils-cuda");

    ui->combo_convert_options->setCurrentIndex((int)config.conversion_element);
}


OptionsDialog::~OptionsDialog()
{}


TcamCaptureConfig OptionsDialog::get_config ()
{
    app_config.conversion_element = (ConversionElement)ui->combo_convert_options->currentIndex();

    return app_config;

}
