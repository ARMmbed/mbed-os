
_python_argcomplete() {
    local IFS=''
    COMPREPLY=( $(IFS="$IFS"                   COMP_LINE="$COMP_LINE"                   COMP_POINT="$COMP_POINT"                   _ARGCOMPLETE_COMP_WORDBREAKS="$COMP_WORDBREAKS"                   _ARGCOMPLETE=1                   "$1" 8>&1 9>&2 1>/dev/null 2>/dev/null) )
    if [[ $? != 0 ]]; then
        unset COMPREPLY
    fi
}
complete -o nospace -o default -F _python_argcomplete "tools/build.py"
complete -o nospace -o default -F _python_argcomplete "tools/make.py"
complete -o nospace -o default -F _python_argcomplete "tools/test.py"
