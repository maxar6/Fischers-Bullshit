/*
 * IOCallbacks.c
 *
 * Code generation for model "BaseEngineController_A02_bak2.mdl".
 *
 * Model version              : 1.1531
 * Simulink Coder version : 8.0 (R2011a) 09-Mar-2011
 * C source code generated on : Tue Jan 01 09:48:38 2019
 *
 * Target selection: motohawk_ert_rtw.tlc
 * Embedded hardware selection: Specified
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "CommonInclude.h"
#include "BaseEngineController_A02_bak2.h"
#include "BaseEngineController_A02_bak2_private.h"

int32_T SparkSeqDiag_24_Count = 0;
int32_T SparkSeqDiag_24_LastPin = -1;
uint8_T SparkSeqDiag_24_Short[8];
uint8_T SparkSeqDiag_24_Open[8];
uint16_T SparkSeqDiag_24_OpenADC[8];
uint16_T SparkSeqDiag_24_OnTime[8];
void SparkSeqDiagCallback_24(S_SeqOutDiagReportPtr report, NativePtrSizedInt_U
  userData)
{
  int32_T pin_number = (int32_T) userData;

  /* Check for short circuit */
  if (report->DiagReportObj.EST.u1ValidDataMask & HAS_SHORT_CCT_EST_DIAG) {
    SparkSeqDiag_24_OnTime[pin_number] =
      report->DiagReportObj.EST.ShortDataObj.u2OnTimeInMicroSecs;
    if (report->DiagReportObj.EST.ShortDataObj.b1Occurred == 0) {
      SparkSeqDiag_24_Short[pin_number] = 1;/* Fault */
    } else {
      SparkSeqDiag_24_Short[pin_number] = 0;/* No Fault */
    }
  } else {
    SparkSeqDiag_24_Short[pin_number] = 2;/* Indeterminate */
  }

  /* Check for open circuit */
  if (report->DiagReportObj.EST.u1ValidDataMask & HAS_OPEN_CCT_EST_DIAG) {
    SparkSeqDiag_24_OpenADC[pin_number] =
      report->DiagReportObj.EST.OpenDataObj.uADCSample;
    if (report->DiagReportObj.EST.OpenDataObj.uADCSample >= 1023) {
      SparkSeqDiag_24_Short[pin_number] = 1;/* Short to Power */
      SparkSeqDiag_24_Open[pin_number] = 0;/* Not Open */
    } else if (report->DiagReportObj.EST.OpenDataObj.uADCSample <= 10) {
      SparkSeqDiag_24_Short[pin_number] = 1;/* Short to Ground */
      SparkSeqDiag_24_Open[pin_number] = 0;/* Not Open */
    } else if (report->DiagReportObj.EST.OpenDataObj.uADCSample >
               (EST_OpenThreshold_DataStore())) {
      SparkSeqDiag_24_Short[pin_number] = 0;/* Not Shorted */
      SparkSeqDiag_24_Open[pin_number] = 1;/* Open Fault */
    } else {
      SparkSeqDiag_24_Short[pin_number] = 0;/* Not Shorted */
      SparkSeqDiag_24_Open[pin_number] = 0;/* Not Open */
    }
  } else {
    SparkSeqDiag_24_Open[pin_number] = 2;/* Indeterminate */
  }

  SparkSeqDiag_24_Count++;
  SparkSeqDiag_24_LastPin = pin_number;
}

void ReSyncAfterCrankUnsync_24(void)
{
  extern NativeError_S ReapplySeqOutCond(E_ModuleResource);
  extern uint8_T motohawk_encoder_fault;
  if (motohawk_encoder_fault == 1) {
    if ((EST_SequenceType_DataStore()) == 2) {
      int i;
      int half_cylinders = (EncoderNumCylinders_DataStore()) / 2;
      for (i=0; i < half_cylinders; i++) {
        ReapplySeqOutCond((E_ModuleResource) ((EST_InitialPin_DataStore()) + i));
      }
    } else if ((EST_SequenceType_DataStore()) == 3) {
      ReapplySeqOutCond((E_ModuleResource) (EST_InitialPin_DataStore()));
    }
  }
}
