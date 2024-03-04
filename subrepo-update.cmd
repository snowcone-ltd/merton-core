git clone git@github.com:stenzek/duckstation duckstation/duckstation
git -C duckstation/duckstation fetch
git -C duckstation/duckstation reset --hard HEAD
git -C duckstation/duckstation checkout faa64ec4ffd08ebb6ee9e378a23717a5c3310d1b

git clone git@github.com:ekeeke/Genesis-Plus-GX genesis-plus-gx/Genesis-Plus-GX
git -C genesis-plus-gx/Genesis-Plus-GX fetch
git -C genesis-plus-gx/Genesis-Plus-GX reset --hard HEAD
git -C genesis-plus-gx/Genesis-Plus-GX checkout 8c72ba0d4557aab4bbbae05a0c6b1382d23b4e74

git clone git@github.com:SourMesen/Mesen2 mesen2/Mesen2
git -C mesen2/Mesen2 fetch
git -C mesen2/Mesen2 reset --hard HEAD
git -C mesen2/Mesen2 checkout f66c319464434c5bf92cea8cf97954dfbcb21979

git clone git@github.com:mgba-emu/mgba mgba/mgba
git -C mgba/mgba fetch
git -C mgba/mgba reset --hard HEAD
git -C mgba/mgba checkout 3571b112dc821d3612e5073afb8b6be41a35dc55

git clone git@github.com:mupen64plus/mupen64plus-core mupen64plus/mupen64plus-core
git -C mupen64plus/mupen64plus-core fetch
git -C mupen64plus/mupen64plus-core reset --hard HEAD
git -C mupen64plus/mupen64plus-core checkout 860fac3fbae94194a392c1d9857e185eda6d083e

git clone git@github.com:ata4/angrylion-rdp-plus mupen64plus/angrylion-rdp-plus
git -C mupen64plus/angrylion-rdp-plus fetch
git -C mupen64plus/angrylion-rdp-plus reset --hard HEAD
git -C mupen64plus/angrylion-rdp-plus checkout 20eaeaffc83ee7a4ca64a533a65f906b82d10dc4

git clone git@github.com:mupen64plus/mupen64plus-rsp-cxd4 mupen64plus/mupen64plus-rsp-cxd4
git -C mupen64plus/mupen64plus-rsp-cxd4 fetch
git -C mupen64plus/mupen64plus-rsp-cxd4 reset --hard HEAD
git -C mupen64plus/mupen64plus-rsp-cxd4 0a4e30f56033396e3ba47ec0fdd7acea3522362a

git clone git@github.com:Themaister/parallel-rdp-standalone mupen64plus/parallel-rdp
git -C mupen64plus/parallel-rdp fetch
git -C mupen64plus/parallel-rdp reset --hard HEAD
git -C mupen64plus/parallel-rdp checkout 37bf2967e3dd18e059cf5563717bb6c88c261f33

git clone git@github.com:libretro/parallel-rsp mupen64plus/parallel-rsp
git -C mupen64plus/parallel-rsp fetch
git -C mupen64plus/parallel-rsp reset --hard HEAD
git -C mupen64plus/parallel-rsp checkout 5b0f81bd08ad6bca177e2713237697b04282dbe4

git clone git@github.com:stella-emu/stella stella/stella
git -C stella/stella fetch
git -C stella/stella reset --hard HEAD
git -C stella/stella checkout a311e1d714db3837ae4c05e2fab0abcf092a2e54
