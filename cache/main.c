#define BUFF_DATA_CAP
#include "buff.h"
#include <libcpuid/libcpuid.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage %s <outfile>\n", argv[0]);
        return 1;
    }
    FILE *out = fopen(argv[1], "w");
    if (!out) {
        fprintf(stderr, "Can't open file %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    struct cpu_id_t id;
    if (cpu_identify(NULL, &id)) {
        fprintf(stderr, "Can't id cpu: %s\n", cpuid_error());
        return 1;
    }

    if (id.l2_cacheline < 0) {
        fprintf(stderr, "Can't determine l2 cache line size.\n");
        return 1;
    }

    int32_t chunk = 2 * id.l2_cacheline;
    int32_t header = sizeof (struct buff_node);
    int32_t data = chunk - header;

    printf(
            "L2 cache line:    %d bytes\n"
            "data chunk:       %d bytes\n"
            "buff_node header: %d bytes\n"
            "buff_node data:   %d bytes\n",
            id.l2_cacheline, chunk, header, data);

    fprintf(out, "%d\n", data);
    fclose(out);
    return 0;
}
