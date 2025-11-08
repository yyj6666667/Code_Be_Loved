###loss_ce 的三种写法
#前两个选择了不同的数学形式
#第三个短凑粗暴，哈哈哈

def loss_ce_1(Z, y):
    ##写法一：直接使用Z_hot,即loss_ce = -log(Z_hot)
    ##       注：Z 即 softmax(hypothesis(x))
    Z_hot = np.zeros(Z.shape[0])
    for i in range(Z.shape[0]):
        index = y[i]
        Z_hot[i] = np.exp(Z[i, index]) / np.sum(np.exp(Z[i]))
    Loss = np.mean(-np.log(Z_hot))
    return Loss

def loss_ce_2(Z, y):
    ##写法二：loss_ce = -h_y(x) + log(\sum exp(h_i)) 
    batch_size = Z.shape[0]
    hypo_y = Z[np.arange(batch_size), y]
    sum = 0
    for j in range(Z.shape[1]):
        sum += np.exp(Z[np.arange(batch_size), j])
    Loss_vector = -(hypo_y) + np.log(sum)
    Loss = np.mean(Loss_vector)
    return Loss

def loss_ce_3(Z, y):
    #探索更短的写法哈哈哈
    Loss_vector = -Z[np.arange(Z.shape[0]), y] + np.log(np.exp(Z).sum(axis=1))
    return Loss_vector.mean()
    ##一行解决问题，优雅！


def softmax_loss(Z, y):
    """
        Z (np.ndarray[np.float32]): 2D numpy array of shape
            (batch_size, num_classes), containing the logit predictions for
            each class.
        y (np.ndarray[np.uint8]): 1D numpy array of shape (batch_size, )
            containing the true label of each example.

    Returns:
        Average softmax loss over the sample.
    """
    ### BEGIN YOUR CODE
    return loss_ce_3(Z, y)
    ### END YOUR CODE