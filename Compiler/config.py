# (C) 2018 University of Bristol. See License.txt

from collections import defaultdict

#INIT_REG_MAX = 655360
INIT_REG_MAX = 1310720
REG_MAX = 2 ** 32
USER_MEM = 8192
TMP_MEM = 8192
TMP_MEM_BASE = USER_MEM
TMP_REG = 3
TMP_REG_BASE = REG_MAX - TMP_REG

P_VALUES = { -1: 2147483713, \
             32: 2147565569, \
             64: 9223372036855103489, \
             128: 172035116406933162231178957667602464769, \
             256: 57896044624266469032429686755131815517604980759976795324963608525438406557697, \
             512: 6703903964971298549787012499123814115273848577471136527425966013026501536706464354255445443244279389455058889493431223951165286470575994074291745908195329 }

BIT_LENGTHS = { -1: 32,
                32: 16,
                64: 16,
                128: 64,
                256: 64,
                512: 64 }

STAT_SEC = { -1: 6,
             32: 6,
             64: 30,
             128: 40,
             256: 40,
             512: 40 }


COST = { 'modp': defaultdict(lambda: 0,
         { 'triple': 0.00020652622883106154,
           'square': 0.00020652622883106154,
           'bit': 0.00020652622883106154,
           'inverse': 0.00020652622883106154,
           'PreMulC': 2 * 0.00020652622883106154,
         }),
         'gf2n': defaultdict(lambda: 0,
         { 'triple': 0.00020716801325875284,
           'square': 0.00020716801325875284,
           'inverse': 0.00020716801325875284,
           'bit': 1.4492753623188405e-07,
           'bittriple': 0.00004828818388140422,
           'bitgf2ntriple': 0.00020716801325875284,
           'PreMulC': 2 * 0.00020716801325875284,
         })
}


try:
    from config_mine import *
except:
    pass
