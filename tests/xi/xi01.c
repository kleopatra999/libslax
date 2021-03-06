/*
 * Copyright (c) 2016-2017, Juniper Networks, Inc.
 * All rights reserved.
 * This SOFTWARE is licensed under the LICENSE provided in the
 * ../Copyright file. By downloading, installing, copying, or otherwise
 * using the SOFTWARE, you agree to be bound by the terms of that
 * LICENSE.
 *
 * Phil Shafer (phil@) June 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>

#include <libpsu/psucommon.h>
#include <libpsu/psulog.h>
#include <parrotdb/pacommon.h>
#include <libxi/xicommon.h>
#include <libxi/xisource.h>

int
main (int argc, char **argv)
{
    const char *opt_filename = NULL;
    int opt_quiet = FALSE;
    int opt_log = FALSE;
    int opt_unescape = FALSE;
    int fd = 0;
    xi_source_flags_t flags = 0;

    for (argc = 1; argv[argc]; argc++) {
	if (strcmp(argv[argc], "file") == 0
	    || strcmp(argv[argc], "input") == 0) {
	    if (argv[argc + 1])
		opt_filename = argv[++argc];
	} else if (strcmp(argv[argc], "quiet") == 0) {
	    opt_quiet = TRUE;
	} else if (strcmp(argv[argc], "unescape") == 0) {
	    opt_unescape = TRUE;
	} else if (strcmp(argv[argc], "line") == 0) {
	    flags |= XPSF_LINE_NO;
	} else if (strcmp(argv[argc], "trim") == 0) {
	    flags |= XPSF_TRIM_WS;
	} else if (strcmp(argv[argc], "log") == 0) {
	    opt_log = TRUE;
	} else if (strcmp(argv[argc], "ignore") == 0) {
	    flags |= XPSF_IGNORE_WS;
	} else if (strcmp(argv[argc], "ignore-comments") == 0) {
	    flags |= XPSF_IGNORE_COMMENTS;
	} else if (strcmp(argv[argc], "ignore-dtd") == 0) {
	    flags |= XPSF_IGNORE_DTD;
	}
    }

    if (opt_log)
	psu_log_enable(TRUE);

    if (opt_filename != NULL) {
	fd = open(opt_filename, O_RDONLY);
	if (fd < 0)
	    err(1, "could not open file: %s", opt_filename);
    }

    xi_source_t *srcp = xi_source_create(fd, flags);
    if (srcp == NULL)
	errx(1, "failed to create source");

    char *data, *rest;
    xi_node_type_t type;
    for (;;) {
	type = xi_source_next_token(srcp, &data, &rest);
	if (0)
	    psu_log("new token: %u [%s] [%s]", type, data ?: "", rest ?: "");

	switch (type) {
	case XI_TYPE_NONE:	/* Unknown type */
	    return 1;

	case XI_TYPE_EOF:	/* End of file */
	    return 0;

	case XI_TYPE_FAIL:	/* Failure mode */
	    return -1;

	case XI_TYPE_TEXT:	/* Text content */
	    if (!opt_quiet) {
		int len;
		if (opt_unescape && data && rest)
		    len = xi_source_unescape(srcp, data, rest - data);
		else len = rest - data;
		printf("data [%.*s]\n", len, data);
	    }
	    break;

	case XI_TYPE_OPEN:	/* Open tag */
	    if (!opt_quiet)
		printf("open tag [%s] [%s]\n", data ?: "", rest ?: "");
	    break;

	case XI_TYPE_EMPTY:	/* Empty tag */
	    if (!opt_quiet)
		printf("empty tag [%s] [%s]\n", data ?: "", rest ?: "");
	    break;

	case XI_TYPE_CLOSE:	/* Close tag */
	    if (!opt_quiet)
		printf("close tag [%s] [%s]\n", data ?: "", rest ?: "");
	    break;

	case XI_TYPE_PI:	/* Processing instruction */
	    if (!opt_quiet)
		printf("pi [%s] [%s]\n", data ?: "", rest ?: "");
	    break;

	case XI_TYPE_DTD:	/* DTD nonsense */
	    if (!opt_quiet)
		printf("dtd [%s] [%s]\n", data ?: "", rest ?: "");
	    break;

	case XI_TYPE_COMMENT:	/* Comment */
	    if (!opt_quiet)
		printf("comment [%s] [%s]\n", data ?: "", rest ?: "");
	    break;

	case XI_TYPE_CDATA:	/* cdata */
	    if (!opt_quiet)
		printf("cdata [%.*s]\n", (int)(rest - data), data);
	    break;
	}
    }

    xi_source_destroy(srcp);

    return 0;
}
