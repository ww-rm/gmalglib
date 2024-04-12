from typing import Tuple, Optional, Callable

SM2_PARAMS_LENGTH: int
SM2_PARAMS_P: bytes
SM2_PARAMS_A: bytes
SM2_PARAMS_B: bytes
SM2_PARAMS_G: Tuple[bytes, bytes]
SM2_PARAMS_N: bytes

SM2_PCMODE_RAW: int
SM2_PCMODE_COMPRESS: int
SM2_PCMODE_MIX: int

SM2_DEFAULT_UID: bytes
SM2_UID_MAX_LENGTH: int
SM2_ENTITYINFO_LENGTH: int

SM2_SK_LENGTH: int
SM2_PK_HALF_LENGTH: int
SM2_PK_FULL_LENGTH: int

SM2_MSG_MAX_LENGTH: int
SM2_SIGN_R_LENGTH: int
SM2_SIGN_S_LENGTH: int
SM2_SIGNATURE_LENGTH: int

SM2_ENCRYPT_C1_HALF_LENGTH: int
SM2_ENCRYPT_C1_FULL_LENGTH: int
SM2_ENCRYPT_C3_LENGTH: int


class SM2:
    """SM2 算法对象."""

    @staticmethod
    def is_sk_valid(sk: bytes) -> bool:
        """测试私钥是否合法."""

    @staticmethod
    def is_pk_valid(pk: bytes) -> bool:
        """测试公钥是否合法"""

    @staticmethod
    def get_pk(sk: bytes) -> bytes:
        """由私钥得到公钥."""

    def __init__(
        self,
        sk: Optional[bytes] = None,
        pk: Optional[bytes] = None,
        uid: bytes = SM2_DEFAULT_UID,
        *,
        pc_mode: int = SM2_PCMODE_RAW,
        rnd_fn: Optional[Callable[[int], bytes]] = None
    ) -> None:
        """SM2 算法对象.

        Args:
            sk: 私钥.
            pk: 公钥.
            uid: 用户 ID.

        Keyword Args:
            pc_mode: 点压缩模式.
            rnd_fn: 可选的用于产生指定长度随机字节函数.
        """

    def generate_keypair(self) -> Tuple[bytes, bytes]:
        """生成密钥对.

        Returns:
            sk: 私钥.
            pk: 公钥.
        """

    def get_entity_info(self) -> bytes:
        """获取实体信息."""

    def sign_digest(self, digest: bytes) -> bytes:
        """对摘要进行签名."""

    def verify_digest(self, digest: bytes, signature: bytes) -> bool:
        """对摘要及其签名验签."""

    def sign(self, message: bytes) -> bytes:
        """对完整消息进行签名."""

    def verify(self, message: bytes, signature: bytes) -> bool:
        """对完整消息及其签名验签."""
