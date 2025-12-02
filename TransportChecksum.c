/**
* @brief 计算16位校验和, for TCP / UDP
*
* @param buf 要计算的数据包
* @param len 要计算的长度
* @return uint16_t 校验和
*/
uint16_t transport_checksum(uint8_t protocol, buf_t *buf, uint8_t *src_ip, uint8_t *dst_ip) {
    int udp_length = buf->len;
    //做备份，不然ip头数据被破坏
    buf_add_header(buf, sizeof(peso_hdr_t));
    char backup[12];
    memcpy(backup, buf->data, 12);
    //fill in
    peso_hdr_t* peso_hdr = (peso_hdr_t*)(buf->data);
    memcpy(peso_hdr->src_ip, src_ip, 4);
    memcpy(peso_hdr->dst_ip, dst_ip, 4);
    peso_hdr_t* tem = peso_hdr;
    tem->placeholder = 0;
    tem->protocol = protocol;
    tem->total_len16 = swap16(udp_length);
    //fill zero in the end
    int flag_if_add_padding = 0;
    if ((buf->len) % 2 == 1) {
        flag_if_add_padding = 1;
        buf_add_padding(buf, 1);
    }
    // calc checksum
    int sum = 0;
    uint16_t* list = (uint16_t*)(buf->data);
    for (int i = 0; i < buf->len / 2; i++)
        sum += list[i];
    int tem_num = sum;
    while(tem_num >> 16 != 0) {
        int tem_int_1 = tem_num >> 16;
        int tem_int_2 = tem_num << 16 >> 16;
        tem_num = tem_int_1 + tem_int_2;
    }
    uint16_t res = (uint16_t)tem_num;
    res = ~res;
    //恢复
    memcpy(buf->data, backup, 12);
    buf_remove_header(buf, sizeof(peso_hdr_t));
    if (flag_if_add_padding == 1) {
        buf_remove_padding(buf, 1);
    }
    return res;
}