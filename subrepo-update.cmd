git clone git@github.com:stenzek/duckstation duckstation/duckstation
git -C duckstation/duckstation fetch
git -C duckstation/duckstation reset --hard HEAD
git -C duckstation/duckstation checkout 5a185654ba5d50ad137a5f7fc4c80854e66addb0

git clone git@github.com:ekeeke/Genesis-Plus-GX genesis-plus-gx/Genesis-Plus-GX
git -C genesis-plus-gx/Genesis-Plus-GX fetch
git -C genesis-plus-gx/Genesis-Plus-GX reset --hard HEAD
git -C genesis-plus-gx/Genesis-Plus-GX checkout f9f16d7a552c5606caea52c2179ca2831b8e0177

git clone git@github.com:SourMesen/Mesen2 mesen2/Mesen2
git -C mesen2/Mesen2 fetch
git -C mesen2/Mesen2 reset --hard HEAD
git -C mesen2/Mesen2 checkout 211b42e91ca323d9a8df4accbd277a187765d127

git clone git@github.com:mgba-emu/mgba mgba/mgba
git -C mgba/mgba fetch
git -C mgba/mgba reset --hard HEAD
git -C mgba/mgba checkout 3571b112dc821d3612e5073afb8b6be41a35dc55

git clone git@github.com:mupen64plus/mupen64plus-core mupen64plus/mupen64plus-core
git -C mupen64plus/mupen64plus-core fetch
git -C mupen64plus/mupen64plus-core reset --hard HEAD
git -C mupen64plus/mupen64plus-core checkout 5340dafcc0f5e8284057ab931dd5c66222d3d49e

git clone git@github.com:ata4/angrylion-rdp-plus mupen64plus/angrylion-rdp-plus
git -C mupen64plus/angrylion-rdp-plus fetch
git -C mupen64plus/angrylion-rdp-plus reset --hard HEAD
git -C mupen64plus/angrylion-rdp-plus checkout 8a1cc860b1afcba8ac80f9569ca46ba0f8fbddd3

git clone git@github.com:mupen64plus/mupen64plus-rsp-cxd4 mupen64plus/mupen64plus-rsp-cxd4
git -C mupen64plus/mupen64plus-rsp-cxd4 fetch
git -C mupen64plus/mupen64plus-rsp-cxd4 reset --hard HEAD
git -C mupen64plus/mupen64plus-rsp-cxd4 checkout f6ff3719cb68d3e1c1497fc87a661921671db719

git clone git@github.com:Themaister/parallel-rdp-standalone mupen64plus/parallel-rdp
git -C mupen64plus/parallel-rdp fetch
git -C mupen64plus/parallel-rdp reset --hard HEAD
git -C mupen64plus/parallel-rdp checkout e8a425fabc3a25d1855d5f217c6857e5e9bf1af2

git clone git@github.com:libretro/parallel-rsp mupen64plus/parallel-rsp
git -C mupen64plus/parallel-rsp fetch
git -C mupen64plus/parallel-rsp reset --hard HEAD
git -C mupen64plus/parallel-rsp checkout 8f1d59d5393326358499edb8ecc8a328c680ce43

git clone git@github.com:stella-emu/stella stella/stella
git -C stella/stella fetch
git -C stella/stella reset --hard HEAD
git -C stella/stella checkout a311e1d714db3837ae4c05e2fab0abcf092a2e54
