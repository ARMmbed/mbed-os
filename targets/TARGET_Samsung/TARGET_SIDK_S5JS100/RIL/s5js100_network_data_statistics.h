#ifndef _S5JS100_NETWORK_DATA_STATISTICS_H_
#define _S5JS100_NETWORK_DATA_STATISTICS_H_
typedef struct {
    int numOfDataSessions;          //!< Number of data sessions
    int dataSessionAttemptsFailed;  //!< Number of data session attempts failed.
    int bytesTxed;
    int bytesRxed;
}s5j_data_statistics_t;

extern s5j_data_statistics_t s5j_data_statistics;

#endif