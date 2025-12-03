savedcmd_character.mod := printf '%s\n'   character.o | awk '!x[$$0]++ { print("./"$$0) }' > character.mod
