"""SM4 算法实现.

Attributes:
    SM4_KEY_LENGTH: 密钥长度
    SM4_BLOCK_LENGTH: 数据分组长度
"""


SM4_KEY_LENGTH: int
SM4_BLOCK_LENGTH: int


class SM4:
    def __init__(self, key: bytes) -> None:
        """SM4 算法对象.

        Args:
            key: 密钥.
        """

    def encrypt(self, block: bytes) -> bytes:
        """加密数据块."""

    def decrypt(self, block: bytes) -> bytes:
        """解密数据块."""
