ZUC_KEY_LENGTH: int
ZUC_IV_LENGTH: int
ZUC_WORD_LENGTH: int


class ZUC:
    """ZUC 算法对象."""

    def __init__(self, key: bytes, iv: bytes) -> None:
        """ZUC 算法对象.

        Args:
            key: 密钥.
            iv: 初始化向量.
        """

    def generate(self) -> bytes:
        """生成伪随机字"""

    def copy(self) -> ZUC:
        """拷贝当前对象及其内部状态."""
