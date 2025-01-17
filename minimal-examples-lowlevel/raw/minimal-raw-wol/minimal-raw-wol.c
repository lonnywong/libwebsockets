/*
 * lws-minimal-raw-wol
 *
 * Written in 2010-2023 by Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * This demonstrates using lws_wol()
 */

#include <libwebsockets.h>
#include <net/if.h>

int main(int argc, const char **argv)
{
	struct lws_context_creation_info info;
	struct lws_context *ctx;
	const char *p, *ip = NULL;
	uint8_t mac[IFHWADDRLEN];
	int ret = 1;

	memset(&info, 0, sizeof info);
	lws_cmdline_option_handle_builtin(argc, argv, &info);

	if ((p = lws_cmdline_option(argc, argv, "-ip")))
		ip = p;

	if (argc < 2) {
		lwsl_user("lws-minimal-raw-wol XX:XX:XX:XX:XX:XX [-ip interface IP]\n");
		goto bail1;
	}

	if (lws_parse_mac(argv[1], mac)) {
		lwsl_user("Failed to parse mac '%s'\n", argv[1]);
		goto bail1;
	}

	ctx = lws_create_context(&info);
	if (!ctx) {
		lwsl_err("lws init failed\n");
		goto bail1;
	}

	if (!lws_wol(ctx, ip, mac))
		ret = 0;

	lws_context_destroy(ctx);

bail1:
	return ret;
}
