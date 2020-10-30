#include "rpctime.h"

long *
gettime_1_svc(void *argp, struct svc_req *rqstp)
{
  static long result;
  time(&result);
  return &result;
}
