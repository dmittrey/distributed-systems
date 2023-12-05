void ChannelTest_ReadFromEmptyChannel_ReturnNegOne();
void ChannelTest_ReadFromOnlyHeaderMessage_ReturnHeaderSize();
void ChannelTest_ReadFromFilledMessage_ReturnPayloadAndHeaderSize();
void ChannelTest_ReadTwoMessages_ReturnPayloadAndHeaderSizeTwice();
void ChannelTest_WriteHeaderMessage_ReturnHeaderSize();
void ChannelTest_WriteFilledMessage_ReturnPayloadSize();

void IpcContextTest_WriteFromFirstToSecond_SucessfulReadFromSecond();
void IpcContextTest_WriteFromFirstToThird_SucessfulReadFromThird();
void IpcContextTest_ReadFromEmptyPipe_ReturnZero();
void IpcContextTest_WriteToOpenedPipe_ReturnMsgSize();
void IpcContextTest_WriteToClosedPipe_ReturnNegVal();

void IpcTest_SendBetweenProcesses_Sucessful();
void IpcTest_SendMulticast_Successful();
void IpcTest_ReceiveAny_Successful();
void IpcTest_ReceiveAnyToHostTwice_Sucessful();
void IpcTest_ReceiveAnyToHostTwice2_Sucessful();
void IpcTest_ReceiveAnyToHostTwice3_Sucessful();

int main() {
    ChannelTest_ReadFromEmptyChannel_ReturnNegOne();
    ChannelTest_ReadFromOnlyHeaderMessage_ReturnHeaderSize();
    ChannelTest_ReadFromFilledMessage_ReturnPayloadAndHeaderSize();
    ChannelTest_ReadTwoMessages_ReturnPayloadAndHeaderSizeTwice();
    ChannelTest_WriteHeaderMessage_ReturnHeaderSize();
    ChannelTest_WriteFilledMessage_ReturnPayloadSize();

    IpcContextTest_WriteFromFirstToSecond_SucessfulReadFromSecond();
    IpcContextTest_WriteFromFirstToThird_SucessfulReadFromThird();
    IpcContextTest_ReadFromEmptyPipe_ReturnZero();
    IpcContextTest_WriteToOpenedPipe_ReturnMsgSize();
    IpcContextTest_WriteToClosedPipe_ReturnNegVal();

    IpcTest_SendBetweenProcesses_Sucessful();
    IpcTest_SendMulticast_Successful();
    IpcTest_ReceiveAny_Successful();
    IpcTest_ReceiveAnyToHostTwice_Sucessful();
    IpcTest_ReceiveAnyToHostTwice2_Sucessful();
    IpcTest_ReceiveAnyToHostTwice3_Sucessful();
}
