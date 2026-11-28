// Enhanced failure handling at Store 1403390D0.
// The original sets connection state 1 before calling its original submission
// routine, but leaves that state pending when the routine returns false.
// Do not modify the submission routine, identity, licence or privilege checks.
extern unsigned int s_connectionState;
unsigned char sub_1401A4DC0(int controller);

unsigned int sub_1403390D0(int controller)
{
    const unsigned int previous = s_connectionState;
    s_connectionState = 1;
    const unsigned char result = sub_1401A4DC0(controller);
    // A synchronous callback may have already changed the state. Preserve
    // its decision; otherwise restore the prior state after failed submission.
    if (!result && s_connectionState == 1)
        s_connectionState = previous;
    return result;
}
