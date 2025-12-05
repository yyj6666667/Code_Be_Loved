void casual_read(int offset, char* out, int size) {
    int offset_for_ddriver = (offset / IO_SZ) * IO_SZ;
    int bias = offset - offset_for_ddriver;
    int size_for_ddriver   = ((size + bias + IO_SZ - 1) / IO_SZ) * IO_SZ;
    char* buf = (char*) malloc(size_for_ddriver);

    ddriver_seek(super.fd, offset_for_ddriver, 0);
    int   iter_num = size_for_ddriver / IO_SZ;
    for (int i = 0; i < iter_num; i++) {
        ddriver_read(super.fd, buf + i * IO_SZ, IO_SZ);
    }

    char* real_start = buf + bias;
    memcpy(out, real_start, size);
    free(buf);
}

void casual_write(int offset, char* input, int size) {
    int offset_for_ddriver = (offset / IO_SZ) * IO_SZ;
    int bias = offset - offset_for_ddriver;
    int size_for_ddriver   = ((size + bias + IO_SZ - 1) / IO_SZ) * IO_SZ;
    int   iter = size_for_ddriver / IO_SZ;
    char* input_dd         = (char*) malloc(size_for_ddriver);
    casual_read(offset_for_ddriver, input_dd, size_for_ddriver);
    memcpy(input_dd + bias, input, size);

    ddriver_seek(super.fd, offset_for_ddriver, 0);
    for(int i = 0; i < iter; i++) {
        ddriver_write(super.fd, input_dd + i * IO_SZ, IO_SZ);
    }

    free(input_dd);
}