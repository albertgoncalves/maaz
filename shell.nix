with import <nixpkgs> {};
mkShell.override { stdenv = llvmPackages_12.stdenv; } {
    buildInputs = [
        feh
        graphviz
        lld_12
        python3
        shellcheck
    ];
    shellHook = ''
        . .shellhook
    '';
}
