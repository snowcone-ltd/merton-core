git clone git@github.com:stenzek/duckstation duckstation/duckstation
git -C duckstation/duckstation fetch
git -C duckstation/duckstation reset --hard HEAD
git -C duckstation/duckstation checkout 5a185654ba5d50ad137a5f7fc4c80854e66addb0

git clone git@github.com:ekeeke/Genesis-Plus-GX genesis-plus-gx/Genesis-Plus-GX
git -C genesis-plus-gx/Genesis-Plus-GX fetch
git -C genesis-plus-gx/Genesis-Plus-GX reset --hard HEAD
git -C genesis-plus-gx/Genesis-Plus-GX checkout 220b22d96fe196488c3f3a2b32175f7f61283800

git clone git@github.com:snowcone-ltd/mednafen mednafen/mednafen
git -C mednafen/mednafen fetch
git -C mednafen/mednafen reset --hard HEAD
git -C mednafen/mednafen checkout f44bdf3cddc4937c273d2ff10087efe0da0399d7

git clone git@github.com:SourMesen/Mesen2 mesen2/Mesen2
git -C mesen2/Mesen2 fetch
git -C mesen2/Mesen2 reset --hard HEAD
git -C mesen2/Mesen2 checkout fabc9a62174f8734a113df6d244f5539ef6b8fcf

git clone git@github.com:mgba-emu/mgba mgba/mgba
git -C mgba/mgba fetch
git -C mgba/mgba reset --hard HEAD
git -C mgba/mgba checkout 3571b112dc821d3612e5073afb8b6be41a35dc55

git clone git@github.com:mupen64plus/mupen64plus-core mupen64plus/mupen64plus-core
git -C mupen64plus/mupen64plus-core fetch
git -C mupen64plus/mupen64plus-core reset --hard HEAD
git -C mupen64plus/mupen64plus-core checkout 614c363d90002c577d26f06a22eb0ef95bf818ad

git clone git@github.com:ata4/angrylion-rdp-plus mupen64plus/angrylion-rdp-plus
git -C mupen64plus/angrylion-rdp-plus fetch
git -C mupen64plus/angrylion-rdp-plus reset --hard HEAD
git -C mupen64plus/angrylion-rdp-plus checkout 9c8b9ed3e7d7f00dff8bc872ccdd3fba1a3673fc

git clone git@github.com:mupen64plus/mupen64plus-rsp-cxd4 mupen64plus/mupen64plus-rsp-cxd4
git -C mupen64plus/mupen64plus-rsp-cxd4 fetch
git -C mupen64plus/mupen64plus-rsp-cxd4 reset --hard HEAD
git -C mupen64plus/mupen64plus-rsp-cxd4 checkout 01c6cd826ebd0eb975b4aca3297170fe6858e49d

git clone git@github.com:Themaister/parallel-rdp-standalone mupen64plus/parallel-rdp
git -C mupen64plus/parallel-rdp fetch
git -C mupen64plus/parallel-rdp reset --hard HEAD
git -C mupen64plus/parallel-rdp checkout 388d70f5835b352d841d9d9e5a08c5de01470f41

git clone git@github.com:libretro/parallel-rsp mupen64plus/parallel-rsp
git -C mupen64plus/parallel-rsp fetch
git -C mupen64plus/parallel-rsp reset --hard HEAD
git -C mupen64plus/parallel-rsp checkout fd28f47a96370c8bfcf41d680d1ab01879a801a8

git clone git@github.com:stella-emu/stella stella/stella
git -C stella/stella fetch
git -C stella/stella reset --hard HEAD
git -C stella/stella checkout a311e1d714db3837ae4c05e2fab0abcf092a2e54
