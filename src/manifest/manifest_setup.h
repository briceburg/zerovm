/*
 * manifest setup API, zerovm help screen, useful defines
 * todo: move COND_ABORT() to appropriate space
 *
 *  Created on: Nov 30, 2011
 *      Author: d'b
 */
#ifndef SERVICE_RUNTIME_MANIFEST_SETUP_H__
#define SERVICE_RUNTIME_MANIFEST_SETUP_H__ 1

EXTERN_C_BEGIN

#include "api/zvm.h"
#include "src/service_runtime/sel_ldr.h"
#include "stdio.h"

#define HELP_SCREEN /* update command line switches here */\
    "ZeroVM lightweight VM manager, build Sat Oct 6 10:05:07\n"\
    "Usage: [-M manifest_file] [-v #] [-l #] [-FSseQ]\n\n"\
    " -M <file> load settings from manifest\n"\
    " -e enable the data oversight (etag)\n"\
    " -l set a new storage limit (in Gb)\n"\
    " -s skip validation\n\n"\
    " (testing flags)\n"\
    " -v [level] verbosity\n"\
    " -F fuzz testing; quit after loading NaCl app\n"\
    " -S disable signal handling\n"\
    " -Q disable platform qualification (dangerous!)\n"

/* todo(d'b): order and move it to zerovm setup or gather all zvm settings here {{ */
#define NEXE_PGM_NAME "loner" /* default argv[0] for nexe */
#define MANIFEST_VERSION "09082012"
#define ZEROVMLOG_NAME "ZeroVM"
#define ZEROVMLOG_OPTIONS (LOG_CONS | LOG_PID | LOG_NDELAY)
#define ZEROVMLOG_PRIORITY LOG_USER
#define ZEROVM_PRIORITY 19
#define ZEROVM_IO_LIMIT_UNIT 0x40000000l
#define ZEROVM_IO_LIMIT 4*ZEROVM_IO_LIMIT_UNIT
#define NEW_ROOT "/"
#define VALIDATOR_NAME "ncval"

/* extended accounting */
#define CGROUPS_FOLDER "/cgroups/zerovm"
#define CGROUPS_TASKS "tasks"
#define CGROUPS_USER_CPU "cpuacct.usage"
#define CGROUPS_MEMORY "memory.max_usage_in_bytes"
#define CGROUPS_SWAP "memory.memsw.max_usage_in_bytes"
/* }} */

/*
 * todo(d'b): make a decition: leave it here or move to NaClApp
 */
struct SystemManifest
{
  /* zerovm control */
  char *version; /* zerovm version */
  int32_t ret_code; /* zerovm return code */
  char *extended_accounting; /* accounting folder name. NULL if not available */

  /* nexe control */
  char *nexe; /* nexe file name */
  int32_t nexe_max; /* max allowed nexe length */
  char *nexe_etag; /* signature. reserved for a future "short" nexe validation */
  unsigned char *etag; /* channels and user memory digital signature */
  int32_t timeout; /* time user module allowed to run */
  int32_t user_ret_code; /* nexe return code */

  /* variables and limits for a nexe */
  int cmd_line_size; /* command line size for nexe */
  char **cmd_line; /* command line for nexe */
  char **envp; /* environment variables for user */
  int32_t syscallback; /* untrusted address callback (see "syscallback.txt") */

  /* channels */
  int32_t channels_count; /* count of channels */
  struct ChannelDesc *channels; /* i/o channels */
};

/*
 * "defense in depth". the last frontier of defense.
 * zerovm limits itself as much as possible
 */
void LastDefenseLine();

/*
 * construct system_manifest object and initialize from manifest
 * todo(d'b): everythig about 'report' should be moved to HostManifestCtor()
 */
void SystemManifestCtor(struct NaClApp *nap);

/* deallocate memory, close files, free other resources. put everything in the place */
int SystemManifestDtor(struct NaClApp *nap);

/*
 * write report for the proxy and free used resources
 * return 0 if success, otherwise - non 0
 */
int ProxyReport(struct NaClApp *nap);

EXTERN_C_END

#endif
