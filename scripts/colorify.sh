set -e

E='([\d\.ms\(\)]+)[ \t]*'

trap "" INT

./run.sh $@ --stdout-nobuffer \
    | colout "^$E$E$E$E$E$E$E" \
        yellow,white,blue,red,red,red,yellow \
        normal,normal,bold,faint,normal,bold,normal \
    | colout 'LANG_\w+' yellow bold \
    | colout '(\(Bin \d+\))' white normal,bold,normal \
    | colout '\[\w+Component\]' yellow faint \
    | colout 'N_elems \d+' yellow normal \
    | colout 'Total \d+\.\d+' white bold \
    | colout 'Rate \d+\.\d+' white normal \
    | colout '\[leaf\]' yellow normal \
    | colout '\[Root\]' yellow normal \
    | colout '\(Entity.*\)' red normal \
    | colout 'dist:.*' red normal \
    | colout 'Parent Node.*' blue normal \
    | colout 'Tweeter = \d+' white bold \
    | colout '\d*------------------------------------------------------------------------' blue bold

