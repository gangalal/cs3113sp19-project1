#!/usr/bin/env bats


TEST5=`cat <<EOF
1
2
3
4
5
6
7
Apple
EOF
`

@test "pipe Test" {
  result="$(./oush batch.oush)"
  [ "$result" = "$TEST5" ]
}

