#!/usr/bin/env bash

build_help() {
	cat <<End-Of-Message
$0 usage: ./build [add <objname>] 
Optional Commands:
     add:   add object to Makefile
End-Of-Message
}
main() {
	if [[ $# -eq 2 ]]; then
		case $1 in
			add) 
				sed "s/^O_SRC.*/& ${2}/" -i Makefile
				;;
			*)
				;;
		esac
	elif [[ $# -eq 0 ]]; then
		make
	else
		build_help
	fi
}
main "$@"

