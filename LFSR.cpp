#include <bits/stdc++.h>
using namespace std;

// Function to convert a string to a binary representation
vector<bool> stringToBinary(const string &text) {
    vector<bool> binary;
    for (char c : text) {
        bitset<8> charBits(c);
        for (int i = 7; i >= 0; --i) {
            binary.push_back(charBits[i]);
        }
    }
    return binary;
}

// Function to convert binary representation to a string
string binaryToString(const vector<bool> &binary) {
    string text;
    for (size_t i = 0; i < binary.size(); i += 8) {
        bitset<8> charBits;
        for (int j = 0; j < 8; ++j) {
            if (i + j < binary.size()) {
                charBits[7 - j] = binary[i + j];
            }
        }
        text += static_cast<char>(charBits.to_ulong());

    }
    return text;
}

// Function to encrypt or decrypt using LFSR
vector<bool> lfsrCrypt(const vector<bool> &input, vector<bool> &state, const vector<int> &taps) {
    vector<bool> output; // to store output bit seq
    vector<bool> currentState = state; //flipflop

    for (bool bit : input) {
        bool newBit = accumulate(taps.begin(), taps.end(), false,[&currentState](bool x, int tap)
        { return x ^ currentState[tap]; });
        // Output is XOR of input bit and new bit from LFSR
        output.push_back(bit ^ newBit);
        // Show current state and the new bit
        for (bool s : currentState)
            cout << s;
        cout << " -> " << newBit << endl;
        // Shift the LFSR
        rotate(currentState.rbegin(), currentState.rbegin() + 1, currentState.rend());
        currentState[0] = newBit;
    }
    state = currentState;
    return output;
}

int main() {
    // Input message
    string message = "aya ehab";

    // m value (maximum is 9)
    int m;
    cout << "Enter the length of LFSR (m value, max 9): ";
    cin >> m;

    vector<int> taps;
    cout << "Enter the positions separated by space (ending with 'x'): ";
    int tap;
    while (cin >> tap) {
        if (tap < 0 || tap >= m) {
            cout << "out of range, try again: ";
            continue;
        }
        taps.push_back(m - 1 - tap);
    }
    // Clear input buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Initial vector
    vector<bool> state(m);
    cout << "Enter the initial vector (" << m << " bits): ";
    for (int i = 0; i < m; ++i) {
        char bit;
        cin >> bit;
        state[i] = bit == '1';
    }

    // Convert message to binary
    vector<bool> messageBinary = stringToBinary(message);

    // Encrypt
    cout << "Encryption steps:\n";
    vector<bool> encryptedBinary = lfsrCrypt(messageBinary, state, taps);
    string encryptedText = binaryToString(encryptedBinary);
    cout << "Encrypted text: " << encryptedText << endl;

    // Reset state for decryption
    cout << "Enter the initial vector again for decryption (" << m << " bits): ";
    for (int i = 0; i < m; ++i) {
        char bit;
        cin >> bit;
        state[i] = bit == '1';
    }

    // Decrypt
    cout << "Decryption steps:\n";
    vector<bool> decryptedBinary = lfsrCrypt(encryptedBinary, state, taps);
    string decryptedText = binaryToString(decryptedBinary);
    cout << "Decrypted text: " << decryptedText << endl;
    return 0;
}