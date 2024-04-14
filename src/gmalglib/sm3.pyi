"""SM3 算法实现.

Attributes:
    SM3_MAX_MSG_BITLEN: 最大消息长度
    SM3_DIGEST_LENGTH: 摘要长度
    SM3_KDF_MAX_LENGTH: 密钥派生最大长度
    SM3_MAC_LENGTH: 消息认证码长度
"""

SM3_MAX_MSG_BITLEN: int
SM3_DIGEST_LENGTH: int
SM3_KDF_MAX_LENGTH: int
SM3_MAC_LENGTH: int


class SM3:
    """SM3 算法对象."""

    def __init__(self, data: bytes = b"") -> None:
        """SM3 算法对象.

        Args:
            data: 初始化数据.
        """

    def update(self, data: bytes) -> None:
        """更新内部状态."""

    def digest(self) -> bytes:
        """获取当前状态哈希值."""

    def reset(self) -> None:
        """重置内部状态."""

    def copy(self) -> SM3:
        """拷贝当前对象及其内部状态."""

    def derive_key(self, klen: int) -> bytes:
        """密钥派生函数."""

    def mac(self, key: bytes) -> bytes:
        """消息认证码."""
