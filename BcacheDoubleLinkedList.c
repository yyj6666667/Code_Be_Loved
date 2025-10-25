/**
 * 选美原因：
 * 1.漂亮的cache通过双向链表管理
 * 2.哈希桶分组减少锁争用的优雅实践
 * 3.优雅的窃取逻辑代码实现
 * 4.流程清晰： 查找是否cached-> 能否替换 -> 能否窃取
 * 喜欢就是喜欢，不找理由了
 */
//Snippets:
// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b;
  int key = blockno % NBUCKETS;
  acquire(&bcache.lock[key]);
  for(b = bcache.head[key].next; b != &bcache.head[key]; b = b->next){
    if(b->dev == dev && b->blockno == blockno){ //如果信息块匹配，就给refcnt++
      b->refcnt++;
      release(&bcache.lock[key]);
      acquiresleep(&b->lock); 
      return b;
    }
  }

  // Not cached.那就替换不怎么使用的块(refcnt==0)
  // LRU策略
added:
  for(b = bcache.head[key].prev; b != &bcache.head[key]; b = b->prev){
    if(b->refcnt == 0) {
      b->dev = dev;
      b->blockno = blockno;
      b->valid = 0;
      b->refcnt = 1;
      release(&bcache.lock[key]);
      acquiresleep(&b->lock);
      return b;
    }
  }
  //窃取逻辑，这也是它能够选入code_be_loved的一大原因
  //to do : stealing
  for (int i = 0; i < NBUCKETS; i++) {
    //check if list has availble one
    int nkey = (key + i) % NBUCKETS;
    struct buf* bb = &bcache.head[nkey];
    for (bb = bcache.head[nkey].next; bb != &bcache.head[nkey]; bb = bb->next){
      if(bb->refcnt == 0){
        //steal 取出
        acquire(&bcache.lock[nkey]);
        bb->prev->next = bb->next;
        bb->next->prev = bb->prev;
        release(&bcache.lock[nkey]);
        //加入到链表头部
        bb->next = bcache.head[key].next;
        bcache.head[key].next->prev = bb;
        bcache.head[key].next = bb;
        bb->prev = &bcache.head[key];

        goto added;
      }
    }
  }
  panic("bget: no buffers");
}