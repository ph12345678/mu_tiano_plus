/** @file
  Implements the EDK II Crypto Protocol/PPI services using the library services
  from BaseCryptLib and TlsLib.

  Copyright (C) Microsoft Corporation. All rights reserved.
  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/TlsLib.h>
#include <Protocol/Crypto.h>

/**
  A macro used to retrieve the FixedAtBuild PcdCryptoServiceFamilyEnable with a
  typecast to its associcted structure type PCD_CRYPTO_SERVICE_FAMILY_ENABLE.
**/
#define EDKII_CRYPTO_PCD(Name) \
  (FixedPcdGetBool (PcdCryptoService##Name))

/**
  A macro used to call a non-void BaseCryptLib function if it is enabled.

  If a BaseCryptLib function is not enabled, there will be no references to it
  from this module and will be optimized away reducing the size of this module.

  @param  Function          The name of the BaseCryptLib function.
  @param  Args              The argument list to pass to Function.
  @param  ErrorReturnValue  The value to return if the BaseCryptLib function is
                            not enabled.

**/
#define CALL_BASECRYPTLIB(Function, Args, ErrorReturnValue) \
  EDKII_CRYPTO_PCD(Function)                                          \
    ? Function Args                                                 \
    : (BaseCryptLibServiceNotEnabled (#Function), ErrorReturnValue)

/**
  A macro used to call a void BaseCryptLib function if it is enabled.

  If a BaseCryptLib function is not enabled, there will be no references to it
  from this module and will be optimized away reducing the size of this module.

  @param  Enable            The name of the enable field in PCD
                            PcdCryptoServiceFamilyEnable for the BaseCryptLib
                            function being called.  If the value of this field
                            is non-zero, then the BaseCryptLib function is
                            enabled.
  @param  Function          The name of the BaseCryptLib function.
  @param  Args              The argument list to pass to Function.

**/
#define CALL_VOID_BASECRYPTLIB( Function, Args)  \
  EDKII_CRYPTO_PCD(Function)                              \
    ? Function Args                                     \
    : BaseCryptLibServiceNotEnabled (#Function)

/**
 * A macro that will check if the PCD is enabled for RELEASE builds
*/
#define NULL_IF_DISABLED(Function)                \
    EDKII_CRYPTO_PCD(Function)                     \
    ? CryptoService##Function                                    \
    : NULL 

/**
  Internal worker function that prints a debug message and asserts if a call is
  made to a BaseCryptLib function that is not enabled in the EDK II Crypto
  Protocol/PPI.

  If this debug message and assert are observed, then a module is using
  BaseCryptLib function that is not enabled in a Crypto driver.  The
  PcdCryptoServiceFamilyEnable should be updated to enable the missing service.

  @param[in]  FunctionName  Null-terminated ASCII string that is the name of an
                            EDK II Crypto service.

**/
static
VOID
BaseCryptLibServiceNotEnabled (
  IN CONST CHAR8  *FunctionName
  )
{
  DEBUG ((DEBUG_ERROR, "[%a] Function %a() is not enabled\n", gEfiCallerBaseName, FunctionName));
  ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
}

/**
  Returns the version of the EDK II Crypto Protocol.

  @return  The version of the EDK II Crypto Protocol.

**/
UINTN
EFIAPI
CryptoServiceGetCryptoVersion (
  VOID
  )
{
  return EDKII_CRYPTO_VERSION;
}
// MU_CHANGE START
<!-- REPLACEMENT -->
// MU_CHANGE END