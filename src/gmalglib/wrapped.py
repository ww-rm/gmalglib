"""算法包装模块, 提供过程式调用方式."""

from . import sm2 as __sm2
from . import sm3 as __sm3
from . import sm4 as __sm4
from . import zuc as __zuc
import typing as __T


def sm3_digest(msg: bytes) -> bytes:
    """计算 SM3 哈希值."""

    return __sm3.SM3(msg).digest()


def sm3_kdf(sbytes: bytes, klen: int) -> bytes:
    """SM3 密钥派生函数."""

    return __sm3.SM3(sbytes).derive_key(klen)


def sm3_mac(msg: bytes, key: bytes) -> bytes:
    """SM3 消息认证码."""

    return __sm3.SM3(msg).mac(key)


def sm4_encrypt(key: bytes, block: bytes) -> bytes:
    """SM4 数据块加密."""

    return __sm4.SM4(key).encrypt(block)


def sm4_decrypt(key: bytes, block: bytes) -> bytes:
    """SM4 数据块解密."""

    return __sm4.SM4(key).decrypt(block)


def zuc_generate(key: bytes, iv: bytes, klen: int) -> bytes:
    """ZUC 伪随机密钥流生成."""

    words = []
    obj = __zuc.ZUC(key, iv)
    n, r = divmod(klen, __zuc.ZUC_WORD_LENGTH)
    for _ in range(n):
        words.append(obj.generate())
    if r > 0:
        words.append(obj.generate()[:r])
    return b"".join(words)


__sm2_pcmode = {"raw": __sm2.SM2_PCMODE_RAW, "compress": __sm2.SM2_PCMODE_COMPRESS, "mix": __sm2.SM2_PCMODE_MIX}


def sm2_is_sk_valid(sk: bytes) -> bool:
    """测试私钥是否合法."""

    return __sm2.SM2.is_sk_valid(sk)


def sm2_is_pk_valid(pk: bytes) -> bool:
    """测试公钥是否合法."""

    return __sm2.SM2.is_pk_valid(pk)


def sm2_get_pk(sk: bytes) -> bytes:
    """由私钥得到公钥."""

    return __sm2.SM2.get_pk(sk)


def sm2_generate_keypair(pc_mode: __T.Literal["raw", "compress", "mix"] = "raw") -> __T.Tuple[bytes, bytes]:
    """生成密钥对.

    Returns:
        sk: 私钥.
        pk: 公钥.
    """

    return __sm2.SM2(pc_mode=__sm2_pcmode[pc_mode]).generate_keypair()


def sm2_get_entity_info(pk: bytes, uid: __T.Optional[bytes] = None) -> bytes:
    """获取实体信息."""

    if uid is None:
        uid = __sm2.SM2_DEFAULT_UID

    return __sm2.SM2(pk=pk, uid=uid).get_entity_info()


def sm2_sign_digest(sk: bytes, digest: bytes, uid: __T.Optional[bytes] = None) -> bytes:
    """对摘要进行签名."""

    if uid is None:
        uid = __sm2.SM2_DEFAULT_UID

    return __sm2.SM2(sk=sk, uid=uid).sign_digest(digest)


def sm2_sign(sk: bytes, msg: bytes, uid: __T.Optional[bytes] = None) -> bytes:
    """对消息进行签名."""

    if uid is None:
        uid = __sm2.SM2_DEFAULT_UID

    return __sm2.SM2(sk=sk, uid=uid).sign(msg)


def sm2_verify_digest(pk: bytes, digest: bytes, uid: __T.Optional[bytes] = None) -> bytes:
    """对摘要进行验签."""

    if uid is None:
        uid = __sm2.SM2_DEFAULT_UID

    return __sm2.SM2(pk=pk, uid=uid).verify_digest(digest)


def sm2_verify(pk: bytes, msg: bytes, uid: __T.Optional[bytes] = None) -> bytes:
    """对消息进行验签."""

    if uid is None:
        uid = __sm2.SM2_DEFAULT_UID

    return __sm2.SM2(pk=pk, uid=uid).verify(msg)


def sm2_encrypt(pk: bytes, plain: bytes, pc_mode: __T.Literal["raw", "compress", "mix"] = "raw") -> bytes:
    """加密数据."""

    return __sm2.SM2(pk=pk, pc_mode=__sm2_pcmode[pc_mode]).encrypt(plain)


def sm2_decrypt(sk: bytes, cipher: bytes) -> bytes:
    """解密数据."""

    return __sm2.SM2(sk=sk).decrypt(cipher)
