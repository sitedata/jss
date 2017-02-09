/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdlib.h>
#include <secerr.h>
#include <prerror.h>
#include <sslerr.h>
#include <jni.h>
#include <nspr.h>

typedef struct {
    PRErrorCode native;
    int java;
} Errcode;

/*
 * This table correlates NSPR and NSS error codes to the enums defined
 * in o.m.util.NativeErrcodes. It must be kept in sync with that class.
 */
static Errcode errcodeTable[] = {
    {PR_OUT_OF_MEMORY_ERROR, 1},
    {PR_BAD_DESCRIPTOR_ERROR, 2},
    {PR_WOULD_BLOCK_ERROR, 3},
    {PR_ACCESS_FAULT_ERROR, 4},
    {PR_INVALID_METHOD_ERROR, 5},
    {PR_ILLEGAL_ACCESS_ERROR, 6},
    {PR_UNKNOWN_ERROR, 7},
    {PR_PENDING_INTERRUPT_ERROR, 8},
    {PR_NOT_IMPLEMENTED_ERROR, 9},
    {PR_IO_ERROR, 10},
    {PR_IO_TIMEOUT_ERROR, 11},
    {PR_IO_PENDING_ERROR, 12},
    {PR_DIRECTORY_OPEN_ERROR, 13},
    {PR_INVALID_ARGUMENT_ERROR, 14},
    {PR_ADDRESS_NOT_AVAILABLE_ERROR, 15},
    {PR_ADDRESS_NOT_SUPPORTED_ERROR, 16},
    {PR_IS_CONNECTED_ERROR, 17},
    {PR_BAD_ADDRESS_ERROR, 18},
    {PR_ADDRESS_IN_USE_ERROR, 19},
    {PR_CONNECT_REFUSED_ERROR, 20},
    {PR_NETWORK_UNREACHABLE_ERROR, 21},
    {PR_CONNECT_TIMEOUT_ERROR, 22},
    {PR_NOT_CONNECTED_ERROR, 23},
    {PR_LOAD_LIBRARY_ERROR, 24},
    {PR_UNLOAD_LIBRARY_ERROR, 25},
    {PR_FIND_SYMBOL_ERROR, 26},
    {PR_INSUFFICIENT_RESOURCES_ERROR, 27},
    {PR_DIRECTORY_LOOKUP_ERROR, 28},
    {PR_TPD_RANGE_ERROR, 29},
    {PR_PROC_DESC_TABLE_FULL_ERROR, 30},
    {PR_SYS_DESC_TABLE_FULL_ERROR, 31},
    {PR_NOT_SOCKET_ERROR, 32},
    {PR_NOT_TCP_SOCKET_ERROR, 33},
    {PR_SOCKET_ADDRESS_IS_BOUND_ERROR, 34},
    {PR_NO_ACCESS_RIGHTS_ERROR, 35},
    {PR_OPERATION_NOT_SUPPORTED_ERROR, 36},
    {PR_PROTOCOL_NOT_SUPPORTED_ERROR, 37},
    {PR_REMOTE_FILE_ERROR, 38},
    {PR_BUFFER_OVERFLOW_ERROR, 39},
    {PR_CONNECT_RESET_ERROR, 40},
    {PR_RANGE_ERROR, 41},
    {PR_DEADLOCK_ERROR, 42},
    {PR_FILE_IS_LOCKED_ERROR, 43},
    {PR_FILE_TOO_BIG_ERROR, 44},
    {PR_NO_DEVICE_SPACE_ERROR, 45},
    {PR_PIPE_ERROR, 46},
    {PR_NO_SEEK_DEVICE_ERROR, 47},
    {PR_IS_DIRECTORY_ERROR, 48},
    {PR_LOOP_ERROR, 49},
    {PR_NAME_TOO_LONG_ERROR, 50},
    {PR_FILE_NOT_FOUND_ERROR, 51},
    {PR_NOT_DIRECTORY_ERROR, 52},
    {PR_READ_ONLY_FILESYSTEM_ERROR, 53},
    {PR_DIRECTORY_NOT_EMPTY_ERROR, 54},
    {PR_FILESYSTEM_MOUNTED_ERROR, 55},
    {PR_NOT_SAME_DEVICE_ERROR, 56},
    {PR_DIRECTORY_CORRUPTED_ERROR, 57},
    {PR_FILE_EXISTS_ERROR, 58},
    {PR_MAX_DIRECTORY_ENTRIES_ERROR, 59},
    {PR_INVALID_DEVICE_STATE_ERROR, 60},
    {PR_DEVICE_IS_LOCKED_ERROR, 61},
    {PR_NO_MORE_FILES_ERROR, 62},
    {PR_END_OF_FILE_ERROR, 63},
    {PR_FILE_SEEK_ERROR, 64},
    {PR_FILE_IS_BUSY_ERROR, 65},
    {PR_OPERATION_ABORTED_ERROR, 66},
    {PR_IN_PROGRESS_ERROR, 67},
    {PR_ALREADY_INITIATED_ERROR, 68},
    {PR_GROUP_EMPTY_ERROR, 69},
    {PR_INVALID_STATE_ERROR, 70},
    {PR_NETWORK_DOWN_ERROR, 71},
    {PR_SOCKET_SHUTDOWN_ERROR, 72},
    {PR_CONNECT_ABORTED_ERROR, 73},
    {PR_HOST_UNREACHABLE_ERROR, 74},
    {SSL_ERROR_BASE, 77},
    {SSL_ERROR_EXPORT_ONLY_SERVER, 78},
    {SSL_ERROR_US_ONLY_SERVER, 79},
    {SSL_ERROR_NO_CYPHER_OVERLAP, 80},
    {SSL_ERROR_NO_CERTIFICATE, 81},
    {SSL_ERROR_BAD_CERTIFICATE, 82},
    {SSL_ERROR_BAD_CLIENT, 83},
    {SSL_ERROR_BAD_SERVER, 84},
    {SSL_ERROR_UNSUPPORTED_CERTIFICATE_TYPE, 85},
    {SSL_ERROR_UNSUPPORTED_VERSION, 86},
    {SSL_ERROR_WRONG_CERTIFICATE, 87},
    {SSL_ERROR_BAD_CERT_DOMAIN, 88},
    {SSL_ERROR_POST_WARNING, 89},
    {SSL_ERROR_SSL2_DISABLED, 90},
    {SSL_ERROR_BAD_MAC_READ, 91},
    {SSL_ERROR_BAD_MAC_ALERT, 92},
    {SSL_ERROR_BAD_CERT_ALERT, 93},
    {SSL_ERROR_REVOKED_CERT_ALERT, 94},
    {SSL_ERROR_EXPIRED_CERT_ALERT, 95},
    {SSL_ERROR_SSL_DISABLED, 96},
    {SSL_ERROR_FORTEZZA_PQG, 97},
    {SSL_ERROR_UNKNOWN_CIPHER_SUITE, 98},
    {SSL_ERROR_NO_CIPHERS_SUPPORTED, 99},
    {SSL_ERROR_BAD_BLOCK_PADDING, 100},
    {SSL_ERROR_RX_RECORD_TOO_LONG, 101},
    {SSL_ERROR_TX_RECORD_TOO_LONG, 102},
    {SSL_ERROR_RX_MALFORMED_HELLO_REQUEST, 103},
    {SSL_ERROR_RX_MALFORMED_CLIENT_HELLO, 104},
    {SSL_ERROR_RX_MALFORMED_SERVER_HELLO, 105},
    {SSL_ERROR_RX_MALFORMED_CERTIFICATE, 106},
    {SSL_ERROR_RX_MALFORMED_SERVER_KEY_EXCH, 107},
    {SSL_ERROR_RX_MALFORMED_CERT_REQUEST, 108},
    {SSL_ERROR_RX_MALFORMED_HELLO_DONE, 109},
    {SSL_ERROR_RX_MALFORMED_CERT_VERIFY, 110},
    {SSL_ERROR_RX_MALFORMED_CLIENT_KEY_EXCH, 111},
    {SSL_ERROR_RX_MALFORMED_FINISHED, 112},
    {SSL_ERROR_RX_MALFORMED_CHANGE_CIPHER, 113},
    {SSL_ERROR_RX_MALFORMED_ALERT, 114},
    {SSL_ERROR_RX_MALFORMED_HANDSHAKE, 115},
    {SSL_ERROR_RX_MALFORMED_APPLICATION_DATA, 116},
    {SSL_ERROR_RX_UNEXPECTED_HELLO_REQUEST, 117},
    {SSL_ERROR_RX_UNEXPECTED_CLIENT_HELLO, 118},
    {SSL_ERROR_RX_UNEXPECTED_SERVER_HELLO, 119},
    {SSL_ERROR_RX_UNEXPECTED_CERTIFICATE, 120},
    {SSL_ERROR_RX_UNEXPECTED_SERVER_KEY_EXCH, 121},
    {SSL_ERROR_RX_UNEXPECTED_CERT_REQUEST, 122},
    {SSL_ERROR_RX_UNEXPECTED_HELLO_DONE, 123},
    {SSL_ERROR_RX_UNEXPECTED_CERT_VERIFY, 124},
    {SSL_ERROR_RX_UNEXPECTED_CLIENT_KEY_EXCH, 125},
    {SSL_ERROR_RX_UNEXPECTED_FINISHED, 126},
    {SSL_ERROR_RX_UNEXPECTED_CHANGE_CIPHER, 127},
    {SSL_ERROR_RX_UNEXPECTED_ALERT, 128},
    {SSL_ERROR_RX_UNEXPECTED_HANDSHAKE, 129},
    {SSL_ERROR_RX_UNEXPECTED_APPLICATION_DATA, 130},
    {SSL_ERROR_RX_UNKNOWN_RECORD_TYPE, 131},
    {SSL_ERROR_RX_UNKNOWN_HANDSHAKE, 132},
    {SSL_ERROR_RX_UNKNOWN_ALERT, 133},
    {SSL_ERROR_CLOSE_NOTIFY_ALERT, 134},
    {SSL_ERROR_HANDSHAKE_UNEXPECTED_ALERT, 135},
    {SSL_ERROR_DECOMPRESSION_FAILURE_ALERT, 136},
    {SSL_ERROR_HANDSHAKE_FAILURE_ALERT, 137},
    {SSL_ERROR_ILLEGAL_PARAMETER_ALERT, 138},
    {SSL_ERROR_UNSUPPORTED_CERT_ALERT, 139},
    {SSL_ERROR_CERTIFICATE_UNKNOWN_ALERT, 140},
    {SSL_ERROR_GENERATE_RANDOM_FAILURE, 141},
    {SSL_ERROR_SIGN_HASHES_FAILURE, 142},
    {SSL_ERROR_EXTRACT_PUBLIC_KEY_FAILURE, 143},
    {SSL_ERROR_SERVER_KEY_EXCHANGE_FAILURE, 144},
    {SSL_ERROR_CLIENT_KEY_EXCHANGE_FAILURE, 145},
    {SSL_ERROR_ENCRYPTION_FAILURE, 146},
    {SSL_ERROR_DECRYPTION_FAILURE, 147},
    {SSL_ERROR_SOCKET_WRITE_FAILURE, 148},
    {SSL_ERROR_MD5_DIGEST_FAILURE, 149},
    {SSL_ERROR_SHA_DIGEST_FAILURE, 150},
    {SSL_ERROR_MAC_COMPUTATION_FAILURE, 151},
    {SSL_ERROR_SYM_KEY_CONTEXT_FAILURE, 152},
    {SSL_ERROR_SYM_KEY_UNWRAP_FAILURE, 153},
    {SSL_ERROR_PUB_KEY_SIZE_LIMIT_EXCEEDED, 154},
    {SSL_ERROR_IV_PARAM_FAILURE, 155},
    {SSL_ERROR_INIT_CIPHER_SUITE_FAILURE, 156},
    {SSL_ERROR_SESSION_KEY_GEN_FAILURE, 157},
    {SSL_ERROR_NO_SERVER_KEY_FOR_ALG, 158},
    {SSL_ERROR_TOKEN_INSERTION_REMOVAL, 159},
    {SSL_ERROR_TOKEN_SLOT_NOT_FOUND, 160},
    {SSL_ERROR_NO_COMPRESSION_OVERLAP, 161},
    {SSL_ERROR_HANDSHAKE_NOT_COMPLETED, 162},
    {SSL_ERROR_BAD_HANDSHAKE_HASH_VALUE, 163},
    {SSL_ERROR_CERT_KEA_MISMATCH, 164},
    {SSL_ERROR_NO_TRUSTED_SSL_CLIENT_CA, 165},
    {SSL_ERROR_SESSION_NOT_FOUND, 166},
    {SSL_ERROR_DECRYPTION_FAILED_ALERT, 167},
    {SSL_ERROR_RECORD_OVERFLOW_ALERT, 168},
    {SSL_ERROR_UNKNOWN_CA_ALERT, 169},
    {SSL_ERROR_ACCESS_DENIED_ALERT, 170},
    {SSL_ERROR_DECODE_ERROR_ALERT, 171},
    {SSL_ERROR_DECRYPT_ERROR_ALERT, 172},
    {SSL_ERROR_EXPORT_RESTRICTION_ALERT, 173},
    {SSL_ERROR_PROTOCOL_VERSION_ALERT, 174},
    {SSL_ERROR_INSUFFICIENT_SECURITY_ALERT, 175},
    {SSL_ERROR_INTERNAL_ERROR_ALERT, 176},
    {SSL_ERROR_USER_CANCELED_ALERT, 177},
    {SSL_ERROR_NO_RENEGOTIATION_ALERT, 178},
    {SEC_ERROR_IO, 179},
    {SEC_ERROR_LIBRARY_FAILURE, 180},
    {SEC_ERROR_BAD_DATA, 181},
    {SEC_ERROR_OUTPUT_LEN, 182},
    {SEC_ERROR_INPUT_LEN, 183},
    {SEC_ERROR_INVALID_ARGS, 184},
    {SEC_ERROR_INVALID_ALGORITHM, 185},
    {SEC_ERROR_INVALID_AVA, 186},
    {SEC_ERROR_INVALID_TIME, 187},
    {SEC_ERROR_BAD_DER, 188},
    {SEC_ERROR_BAD_SIGNATURE, 189},
    {SEC_ERROR_EXPIRED_CERTIFICATE, 190},
    {SEC_ERROR_REVOKED_CERTIFICATE, 191},
    {SEC_ERROR_UNKNOWN_ISSUER, 192},
    {SEC_ERROR_BAD_KEY, 193},
    {SEC_ERROR_BAD_PASSWORD, 194},
    {SEC_ERROR_RETRY_PASSWORD, 195},
    {SEC_ERROR_NO_NODELOCK, 196},
    {SEC_ERROR_BAD_DATABASE, 197},
    {SEC_ERROR_NO_MEMORY, 198},
    {SEC_ERROR_UNTRUSTED_ISSUER, 199},
    {SEC_ERROR_UNTRUSTED_CERT, 200},
    {SEC_ERROR_DUPLICATE_CERT, 201},
    {SEC_ERROR_DUPLICATE_CERT_NAME, 202},
    {SEC_ERROR_ADDING_CERT, 203},
    {SEC_ERROR_FILING_KEY, 204},
    {SEC_ERROR_NO_KEY, 205},
    {SEC_ERROR_CERT_VALID, 206},
    {SEC_ERROR_CERT_NOT_VALID, 207},
    {SEC_ERROR_CERT_NO_RESPONSE, 208},
    {SEC_ERROR_EXPIRED_ISSUER_CERTIFICATE, 209},
    {SEC_ERROR_CRL_EXPIRED, 210},
    {SEC_ERROR_CRL_BAD_SIGNATURE, 211},
    {SEC_ERROR_CRL_INVALID, 212},
    {SEC_ERROR_EXTENSION_VALUE_INVALID, 213},
    {SEC_ERROR_EXTENSION_NOT_FOUND, 214},
    {SEC_ERROR_CA_CERT_INVALID, 215},
    {SEC_ERROR_PATH_LEN_CONSTRAINT_INVALID, 216},
    {SEC_ERROR_CERT_USAGES_INVALID, 217},
    {SEC_INTERNAL_ONLY, 218},
    {SEC_ERROR_INVALID_KEY, 219},
    {SEC_ERROR_UNKNOWN_CRITICAL_EXTENSION, 220},
    {SEC_ERROR_OLD_CRL, 221},
    {SEC_ERROR_NO_EMAIL_CERT, 222},
    {SEC_ERROR_NO_RECIPIENT_CERTS_QUERY, 223},
    {SEC_ERROR_NOT_A_RECIPIENT, 224},
    {SEC_ERROR_PKCS7_KEYALG_MISMATCH, 225},
    {SEC_ERROR_PKCS7_BAD_SIGNATURE, 226},
    {SEC_ERROR_UNSUPPORTED_KEYALG, 227},
    {SEC_ERROR_DECRYPTION_DISALLOWED, 228},
    {XP_SEC_FORTEZZA_BAD_CARD, 229},
    {XP_SEC_FORTEZZA_NO_CARD, 230},
    {XP_SEC_FORTEZZA_NONE_SELECTED, 231},
    {XP_SEC_FORTEZZA_MORE_INFO, 232},
    {XP_SEC_FORTEZZA_PERSON_NOT_FOUND, 233},
    {XP_SEC_FORTEZZA_NO_MORE_INFO, 234},
    {XP_SEC_FORTEZZA_BAD_PIN, 235},
    {XP_SEC_FORTEZZA_PERSON_ERROR, 236},
    {SEC_ERROR_NO_KRL, 237},
    {SEC_ERROR_KRL_EXPIRED, 238},
    {SEC_ERROR_KRL_BAD_SIGNATURE, 239},
    {SEC_ERROR_REVOKED_KEY, 240},
    {SEC_ERROR_KRL_INVALID, 241},
    {SEC_ERROR_NEED_RANDOM, 242},
    {SEC_ERROR_NO_MODULE, 243},
    {SEC_ERROR_NO_TOKEN, 244},
    {SEC_ERROR_READ_ONLY, 245},
    {SEC_ERROR_NO_SLOT_SELECTED, 246},
    {SEC_ERROR_CERT_NICKNAME_COLLISION, 247},
    {SEC_ERROR_KEY_NICKNAME_COLLISION, 248},
    {SEC_ERROR_SAFE_NOT_CREATED, 249},
    {SEC_ERROR_BAGGAGE_NOT_CREATED, 250},
    {XP_JAVA_REMOVE_PRINCIPAL_ERROR, 251},
    {XP_JAVA_DELETE_PRIVILEGE_ERROR, 252},
    {XP_JAVA_CERT_NOT_EXISTS_ERROR, 253},
    {SEC_ERROR_BAD_EXPORT_ALGORITHM, 254},
    {SEC_ERROR_EXPORTING_CERTIFICATES, 255},
    {SEC_ERROR_IMPORTING_CERTIFICATES, 256},
    {SEC_ERROR_PKCS12_DECODING_PFX, 257},
    {SEC_ERROR_PKCS12_INVALID_MAC, 258},
    {SEC_ERROR_PKCS12_UNSUPPORTED_MAC_ALGORITHM, 259},
    {SEC_ERROR_PKCS12_UNSUPPORTED_TRANSPORT_MODE, 260},
    {SEC_ERROR_PKCS12_CORRUPT_PFX_STRUCTURE, 261},
    {SEC_ERROR_PKCS12_UNSUPPORTED_PBE_ALGORITHM, 262},
    {SEC_ERROR_PKCS12_UNSUPPORTED_VERSION, 263},
    {SEC_ERROR_PKCS12_PRIVACY_PASSWORD_INCORRECT, 264},
    {SEC_ERROR_PKCS12_CERT_COLLISION, 265},
    {SEC_ERROR_USER_CANCELLED, 266},
    {SEC_ERROR_PKCS12_DUPLICATE_DATA, 267},
    {SEC_ERROR_MESSAGE_SEND_ABORTED, 268},
    {SEC_ERROR_INADEQUATE_KEY_USAGE, 269},
    {SEC_ERROR_INADEQUATE_CERT_TYPE, 270},
    {SEC_ERROR_CERT_ADDR_MISMATCH, 271},
    {SEC_ERROR_PKCS12_UNABLE_TO_IMPORT_KEY, 272},
    {SEC_ERROR_PKCS12_IMPORTING_CERT_CHAIN, 273},
    {SEC_ERROR_PKCS12_UNABLE_TO_LOCATE_OBJECT_BY_NAME, 274},
    {SEC_ERROR_PKCS12_UNABLE_TO_EXPORT_KEY, 275},
    {SEC_ERROR_PKCS12_UNABLE_TO_WRITE, 276},
    {SEC_ERROR_PKCS12_UNABLE_TO_READ, 277},
    {SEC_ERROR_PKCS12_KEY_DATABASE_NOT_INITIALIZED, 278},
    {SEC_ERROR_KEYGEN_FAIL, 279},
    {SEC_ERROR_INVALID_PASSWORD, 280},
    {SEC_ERROR_RETRY_OLD_PASSWORD, 281},
    {SEC_ERROR_BAD_NICKNAME, 282},
    {SEC_ERROR_NOT_FORTEZZA_ISSUER, 283},
    {SEC_ERROR_CANNOT_MOVE_SENSITIVE_KEY, 284},
    {SEC_ERROR_JS_INVALID_MODULE_NAME, 285},
    {SEC_ERROR_JS_INVALID_DLL, 286},
    {SEC_ERROR_JS_ADD_MOD_FAILURE, 287},
    {SEC_ERROR_JS_DEL_MOD_FAILURE, 288},
    {SEC_ERROR_OLD_KRL, 289},
    {SEC_ERROR_CKL_CONFLICT, 290},
    {SEC_ERROR_CERT_NOT_IN_NAME_SPACE, 291},
    {SEC_ERROR_KRL_NOT_YET_VALID, 292},
    {SEC_ERROR_CRL_NOT_YET_VALID, 293},
    {SEC_ERROR_UNKNOWN_CERT, 294},
    {SEC_ERROR_UNKNOWN_SIGNER, 295},
    {SEC_ERROR_CERT_BAD_ACCESS_LOCATION, 296},
    {SEC_ERROR_OCSP_UNKNOWN_RESPONSE_TYPE, 297},
    {SEC_ERROR_OCSP_BAD_HTTP_RESPONSE, 298},
    {SEC_ERROR_OCSP_MALFORMED_REQUEST, 299},
    {SEC_ERROR_OCSP_SERVER_ERROR, 300},
    {SEC_ERROR_OCSP_TRY_SERVER_LATER, 301},
    {SEC_ERROR_OCSP_REQUEST_NEEDS_SIG, 302},
    {SEC_ERROR_OCSP_UNAUTHORIZED_REQUEST, 303},
    {SEC_ERROR_OCSP_UNKNOWN_RESPONSE_STATUS, 304},
    {SEC_ERROR_OCSP_UNKNOWN_CERT, 305},
    {SEC_ERROR_OCSP_NOT_ENABLED, 306},
    {SEC_ERROR_OCSP_NO_DEFAULT_RESPONDER, 307},
    {SEC_ERROR_OCSP_MALFORMED_RESPONSE, 308},
    {SEC_ERROR_OCSP_UNAUTHORIZED_RESPONSE, 309},
    {SEC_ERROR_OCSP_FUTURE_RESPONSE, 310},
    {SEC_ERROR_OCSP_OLD_RESPONSE, 311},
    {SEC_ERROR_DIGEST_NOT_FOUND, 312},
    {SEC_ERROR_UNSUPPORTED_MESSAGE_TYPE, 313}
};

#define numErrcodes (sizeof(errcodeTable)/sizeof(Errcode))

static int
errcodeCompare(const void *elem1, const void *elem2)
{
    Errcode *ec1, *ec2;

    ec1 = (Errcode*) elem1;
    ec2 = (Errcode*) elem2;

    if( ec1->native < ec2->native ) {
        return -1;
    } else if( ec1->native == ec2->native ) {
        return 0;
    } else {
        /* ec1->native > ec2->native */
        return 1;
    }
}

#ifdef DEBUG
static int initialized = 0;
#endif

/************************************************************************
**
** J S S _ i n i t E r r c o d e T r a n s l a t i o n T a b l e.
**
** Initializes the error code translation table. This should be called
** by CryptoManager.initialize(), and must be called before any calls to
** JSS_ConvertNativeErrcodeToJava.
**
*/
void
JSS_initErrcodeTranslationTable() {
    /* sort the table by native errcode */
    qsort(errcodeTable, numErrcodes, sizeof(Errcode), errcodeCompare);
#ifdef DEBUG
    initialized = 1;
#endif
}

/************************************************************************
**
** J S S _ C o n v e r t N a t i v e E r r c o d e T o J a v a
**
** Converts an NSPR or NSS error code to a Java error code.
** (defined in the class o.m.util.NativeErrcodes)
**
** Returns
**  The Java error code, or -1 if a corresponding Java error code could
**  not be found.
*/
int
JSS_ConvertNativeErrcodeToJava(PRErrorCode nativeErrcode) {
    Errcode key;
    Errcode *target;

#ifdef DEBUG
    PR_ASSERT(initialized);
#endif

    key.native = nativeErrcode;
    target = bsearch( &key, errcodeTable, numErrcodes, sizeof(Errcode),
        errcodeCompare );

    if( target == NULL ) {
        return -1;
    } else {
        return target->java;
    }
}
