with import <nixpkgs> {};
pkgsMusl.mkShell {
    buildInputs = [
        clang_10
        cppcheck
        feh
        graphviz
        python38
        shellcheck
    ];
    shellHook = ''
        . .shellhook
    '';
}
