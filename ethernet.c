#include "ethernet.h"

#include "arp.h"
#include "driver.h"
#include "ip.h"
#include "utils.h"
/**
 * @brief 处理一个收到的数据包
 * 
 * @param buf 要处理的数据包
 */
void ethernet_in(buf_t *buf) {
    // TO-DO
    int least_length = sizeof (ether_hdr_t);
    if(buf->len < least_length || buf == NULL) {
        return;
    } else {
        ether_hdr_t* hdr  = (ether_hdr_t*) buf->data; //这个很妙
        uint16_t protocol = swap16(hdr->protocol16);
        uint8_t* src = hdr->src;
        buf_remove_header(buf, sizeof(ether_hdr_t));
        net_in(buf, protocol, src);
    }
}
/**
 * @brief 处理一个要发送的数据包
 *
 * @param buf 要处理的数据包
 * @param mac 目标MAC地址
 * @param protocol 上层协议
 */
void ethernet_out(buf_t *buf, const uint8_t *mac, net_protocol_t protocol) {
    // TO-DO
    /** 
     * @param 这里隐藏了一个丑陋的方法一，
     * @param 使用malloc
     * @param 多调用了内存
    */
    /*
    if (buf->len < 46) {
        buf_add_padding(buf,46 - buf->len);
    }
    if(buf_add_header(buf, sizeof (ether_hdr_t)) == -1){
        return;
    }
    ether_hdr_t *hdr = NULL;
    hdr = (ether_hdr_t*) malloc(sizeof(ether_hdr_t));
    if (hdr == NULL) return;
    uint8_t src[6] = NET_IF_MAC;
    memcpy(hdr->src, src, NET_MAC_LEN);
    memcpy(hdr->dst, mac, NET_MAC_LEN);
    uint16_t protocol_new = protocol; 
    protocol_new = swap16(protocol_new);
    uint16_t* ptr_tem = &protocol_new;
    memcpy(&hdr->protocol16, ptr_tem, sizeof(uint16_t));
    memcpy(buf->data, hdr, sizeof(ether_hdr_t));
    driver_send(buf);
    free(hdr);
    */

    //以下是最佳实践
    if (buf->len < 46) {
    buf_add_padding(buf,46 - buf->len);
    }
    if(buf_add_header(buf, sizeof (ether_hdr_t)) == -1){
        return;
    }
    ether_hdr_t* hdr = (ether_hdr_t*) buf->data;
    uint8_t src[6] = NET_IF_MAC;
    memcpy(hdr->src, src, 6);
    memcpy(hdr->dst, mac, 6);
    uint16_t protocol_new = protocol; 
    protocol_new = swap16(protocol_new);
    hdr->protocol16 = protocol_new;
    driver_send(buf);
}
/**
 * @brief 初始化以太网协议
 *
 */
void ethernet_init() {
    buf_init(&rxbuf, ETHERNET_MAX_TRANSPORT_UNIT + sizeof(ether_hdr_t));
}

/**
 * @brief 一次以太网轮询
 *
 */
void ethernet_poll() {
    if (driver_recv(&rxbuf) > 0)
        ethernet_in(&rxbuf);
}
