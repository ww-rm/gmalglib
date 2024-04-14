"""SM2 算法实现.

Attributes:
    SM2_PARAMS_LENGTH: SM2 参数长度
    SM2_PARAMS_P: SM2 参数 p
    SM2_PARAMS_A: SM2 参数 a
    SM2_PARAMS_B: SM2 参数 b
    SM2_PARAMS_G: SM2 参数 G
    SM2_PARAMS_N: SM2 参数 n

    SM2_PCMODE_RAW: 未压缩模式
    SM2_PCMODE_COMPRESS: 压缩模式
    SM2_PCMODE_MIX: 混合模式

    SM2_DEFAULT_UID: 默认 uid
    SM2_UID_MAX_LENGTH: uid 最大长度
    SM2_ENTITYINFO_LENGTH: 实体信息长度

    SM2_SK_LENGTH: 私钥长度
    SM2_PK_HALF_LENGTH: 压缩公钥长度
    SM2_PK_FULL_LENGTH: 未压缩公钥长度

    SM2_MSG_MAX_LENGTH: 签名消息最大长度
    SM2_SIGN_R_LENGTH: 签名值 r 长度
    SM2_SIGN_S_LENGTH: 签名值 s 长度
    SM2_SIGNATURE_LENGTH: 签名长度

    SM2_ENCRYPT_C1_HALF_LENGTH: 密文 C1 压缩长度
    SM2_ENCRYPT_C1_FULL_LENGTH: 密文 C1 未压缩长度
    SM2_ENCRYPT_C3_LENGTH: 密文 C3 长度
"""


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
        """测试公钥是否合法."""

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
            pk: 公钥, sk 参数有效时忽略 pk.
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

    def encrypt(self, plain: bytes) -> bytes:
        """加密数据."""

    def decrypt(self, cipher: bytes) -> bytes:
        """解密数据."""
