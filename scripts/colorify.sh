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
    | colout 'Rate \d+\.\d+' white bold \
    | colout '\[leaf\]' yellow normal \
    | colout '\[Root\]' yellow normal \
    | colout '\(Agent.*\)' red normal \
    | colout 'dist:.*' red normal \
    | colout 'Parent Node.*' blue normal \
    | colout 'Tweeter = \d+' white bold \
    | colout '\d*------------------------------------------------------------------------' blue bold \
    | colout '\*\*\*\* START.*\*\*\*\*' white \
    | colout '\*\*\*\* END.*\*\*\*\*' white \
    | colout '(func )(.*)(\..*):' yellow,white,yellow faint,faint,faint \
    | colout '(func )(.*):' yellow,white faint,faintl \
    | colout '\#Time.*Spent' blue bold \
    | colout 'Analysis complete!' white faint \
    | colout '^Simulation.*:.*' white  \
    | colout 'Analysis took .*ms\.' white faint \
    | colout '(average\s+)([\.\d]+)(ms)' cyan,white,cyan normal,normal,normal \
    | colout '(total\s+)([\.\d]+)(ms)' cyan,white,cyan normal,normal,normal \
    | colout '>>' white bold \
    | colout '(calls\s+)([\.\d]+)' cyan,white normal,normal \
    | colout '(max\s+)([\.\d]+)(ms)' cyan,white,cyan normal,normal,normal \
    | colout '(std\.dev\s+)\+-([\.\d]+)(ms), \+-([\.\d]+%)' cyan,white,cyan,red normal,normal,normal,bold

