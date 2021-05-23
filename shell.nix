with import <nixpkgs> {};
mkShell.override { stdenv = llvmPackages_11.stdenv; } {
    buildInputs = [
        cppcheck
        feh
        graphviz
        python3
        shellcheck
    ];
    shellHook = ''
        . .shellhook
    '';
}
