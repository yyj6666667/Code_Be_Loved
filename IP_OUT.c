/**
 * 一段优雅的代码实践
 * IP数据包分组
 */
void ip_out(buf_t *buf, uint8_t *ip, net_protocol_t protocol) {
    int max_pkg_len = (1500 - 4 * ((ip_hdr_t*)buf->data)->hdr_len) & ~7;
    if (buf->len < max_pkg_len) {
        ip_fragment_out(buf, ip, protocol, 0, 0, 0);
    } else {
        int iter = (buf->len + max_pkg_len - 1) / max_pkg_len;
        for (int offset = 0; offset < buf->len; offset += max_pkg_len) {
            int frag_len = ((buf->len - offset) < max_pkg_len) ? buf->len - offset : max_pkg_len;
            buf_t *frag_buf = NULL;
            buf_init(frag_buf, frag->len);
            memcpy(frag_buf, buf->data + offset, frag_len);
            int flag = 1;
            if (offset + max_pkg_len >= buf->len) // last MD set to 0
                flag = 0;
            ip_fragment_out(frag_buf, ip, protocol, 0, offset, flag);
        }
    }
}