/*!
 * \file     sharedflags.h
 * \brief    Shared Flags
 * \author   CoC BRS
 *
 * \copyright
 * Copyright (c) Continental AG and subsidiaries 2015\n
 * All rights reserved\n
 * The reproduction, transmission or use of this document or its contents is
 * not permitted without express written authority.\n
 * Offenders will be liable for damages. All rights, including rights created
 * by patent grant or registration of a utility model or design, are reserved.
 */

#ifndef SHAREDFLAGS
#define SHAREDFLAGS

enum RadioMode {
    FM_MODE,
    AM_MODE,
    DAB_MODE,
    NONE
};

extern int radioMode;

#endif // SHAREDFLAGS

