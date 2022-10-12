#define BUFF_DATA_CAP
#include "buff.h"
#include <libcpuid/libcpuid.h>
#include <stdio.h>

int main(void) {
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

    fprintf(stderr,
            "L2 cache line:    %d bytes\n"
            "data chunk:       %d bytes\n"
            "buff_node header: %d bytes\n"
            "buff_node data:   %d bytes\n",
            id.l2_cacheline, chunk, header, data);

    printf("%d\n", data);
    return 0;
}
