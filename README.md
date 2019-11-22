# Delta4Ts-benchmark

Users can extract a faulty program by using a “define” macro, which also works for multi-fault versions.
For printtokens, Users can copy "print_tokens.c" and "FaultSeeds.print_tokens.h" to the test folder, and remame "FaultSeeds.print_tokens.h" as "FaultSeeds.h". Users can use  “define” macro in "FaultSeeds.h" to get different-fault versions.
In script of EXAMPLE-printtokens folder, users can use "./run.sh 1 2" command to test print_tokens.c with fault 1 and fault 2.
