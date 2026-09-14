{
  description = "算法训练：C++26、libc++ modules、GoogleTest";

  # Same package revision as ~/Documents/interview/flake.lock.
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/d2f67949798825fe853f7c5d0492b8bf016d3f88";

  outputs = { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      llvm = pkgs.llvmPackages_22;
      compiler = llvm.libcxxClang;
      stdenv = llvm.libcxxStdenv;
      gtest = pkgs.gtest.override { inherit stdenv; };
      clangTools = llvm.clang-tools.override { enableLibcxx = true; libcxxClang = compiler; };
    in {
      devShells.${system}.default = pkgs.mkShell.override { inherit stdenv; } {
        packages = [ pkgs.cmake pkgs.ninja clangTools gtest ];
        env.NIX_CXX_STDLIB_MODULES_JSON = "${llvm.libcxx}/lib/libc++.modules.json";
        shellHook = ''
          export CC=clang
          export CXX=clang++
          echo "算法练习环境已就绪。运行 ./judge smoke 检查环境；./judge 01 评测第一关。" >&2
          # fish foreign-env misparses the exported multiline hook as variables.
          unset shellHook
        '';
      };
    };
}
