// SPDX-License-Identifier: GPL-2.0+

#pragma once

#include <vector>

#include <QMutex>
#include <QThread>
#include <memory>

#include <fftw3.h>

#include "dsosamples.h"
#include "postprocessingsettings.h"
#include "ppresult.h"
#include "utils/printutils.h"

#include "processor.h"

class DsoSettings;
struct DsoSettingsScope;

/// \brief Analyzes the data from the dso.
/// Calculates the spectrum and various data about the signal and saves the
/// time-/frequencysteps between two values.
class SpectrumGenerator : public Processor {

  public:
    SpectrumGenerator( const DsoSettingsScope *scope, const DsoSettingsPostProcessing *postprocessing );
    ~SpectrumGenerator() override;

  private:
    const DsoSettingsScope *scope;
    const DsoSettingsPostProcessing *postprocessing;
    unsigned int lastRecordLength = 0;                          ///< The record length of the previously analyzed data
    Dso::WindowFunction lastWindow = Dso::WindowFunction( -1 ); ///< The previously used dft window function
    double *lastWindowBuffer = nullptr;
    fftw_plan fftPlan_R2HC = nullptr;
    fftw_plan fftPlan_HC2R = nullptr;
    // Processor interface
    void process( PPresult *data ) override;
};

// reuse fft plans with new-array fft functions
// takes one second during 1st run but gives faster transforms
#define REUSE_FFTW_PLAN
