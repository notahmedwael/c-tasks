#include "DynamicString.h"

// Helper functions

// copy src into this->data with size len
void DynamicString::copyFrom(const char* src, size_t len) {
    data = new char[len + 1];
    for (size_t i = 0; i < len; i++)
        data[i] = src[i];
    data[len] = '\0';
    length = len;
}

// compare chars A-Z, a-z manually (ASCII ordering)
int DynamicString::charCompare(char a, char b) const {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

// Constructors

DynamicString::DynamicString() : data(nullptr), length(0) {
    data = new char[1];
    data[0] = '\0';
}

DynamicString::DynamicString(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') ++len;
    copyFrom(str, len);
}

DynamicString::DynamicString(const DynamicString& other) {
    copyFrom(other.data, other.length);
}

DynamicString::~DynamicString() {
    delete[] data;
}

// Copy assignment

DynamicString& DynamicString::operator=(const DynamicString& other) {
    if (this != &other) {
        delete[] data;
        copyFrom(other.data, other.length);
    }
    return *this;
}

// Basic operations

size_t DynamicString::size() const { return length; }
bool DynamicString::empty() const { return length == 0; }
const char* DynamicString::c_str() const { return data; }

void DynamicString::clear() {
    delete[] data;
    data = new char[1];
    data[0] = '\0';
    length = 0;
}

void DynamicString::push_back(char c) {
    char* newData = new char[length + 2];
    for (size_t i = 0; i < length; ++i) newData[i] = data[i];
    newData[length] = c;
    newData[length + 1] = '\0';
    delete[] data;
    data = newData;
    ++length;
}

void DynamicString::pop_back() {
    if (length == 0) return;
    --length;
    data[length] = '\0';
}

// append / insert / erase / reverse / compare

void DynamicString::append(const char* s) {
    // compute len of s
    size_t sLen = 0;
    while (s[sLen] != '\0') ++sLen;

    // allocate new buffer
    char* newData = new char[length + sLen + 1];
    // copy existing
    for (size_t i = 0; i < length; ++i) newData[i] = data[i];
    // copy appended
    for (size_t i = 0; i < sLen; ++i) newData[length + i] = s[i];
    newData[length + sLen] = '\0';

    delete[] data;
    data = newData;
    length += sLen;
}

void DynamicString::insert(int index, const char* s) {
    if (index < 0) index = 0;
    if (index > static_cast<int>(length)) index = static_cast<int>(length);

    size_t sLen = 0;
    while (s[sLen] != '\0') ++sLen;

    char* newData = new char[length + sLen + 1];

    // copy prefix [0..index-1]
    for (int i = 0; i < index; ++i) newData[i] = data[i];

    // copy inserted string
    for (size_t j = 0; j < sLen; ++j) newData[index + j] = s[j];

    // copy suffix [index..length-1]
    for (size_t k = index; k < length; ++k) newData[sLen + k] = data[k];

    newData[length + sLen] = '\0';

    delete[] data;
    data = newData;
    length += sLen;
}

void DynamicString::erase(int pos, int len) {
    if (pos < 0) pos = 0;
    if (pos > static_cast<int>(length)) return; // nothing to erase
    if (len <= 0) return;

    int maxRemovable = static_cast<int>(length) - pos;
    if (len > maxRemovable) len = maxRemovable;

    char* newData = new char[length - len + 1];

    // copy [0..pos-1]
    for (int i = 0; i < pos; ++i) newData[i] = data[i];

    // copy [pos+len .. length-1] to position pos
    size_t writePos = pos;
    for (int k = pos + len; k < static_cast<int>(length); ++k) {
        newData[writePos++] = data[k];
    }

    newData[length - len] = '\0';

    delete[] data;
    data = newData;
    length -= static_cast<size_t>(len);
}

void DynamicString::reverse() {
    if (length <= 1) return;
    size_t i = 0, j = length - 1;
    while (i < j) {
        char tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
        ++i;
        --j;
    }
}

int DynamicString::compare(const char* s) const {
    // compute len of s
    size_t sLen = 0;
    while (s[sLen] != '\0') ++sLen;

    size_t minLen = (length < sLen ? length : sLen);
    for (size_t i = 0; i < minLen; ++i) {
        if (data[i] < s[i]) return -1;
        if (data[i] > s[i]) return 1;
    }
    if (length < sLen) return -1;
    if (length > sLen) return 1;
    return 0;
}

// Custom methods

void DynamicString::toLower() {
    for (size_t i = 0; i < length; ++i)
        if (data[i] >= 'A' && data[i] <= 'Z')
            data[i] = static_cast<char>(data[i] + 32);
}

void DynamicString::toUpper() {
    for (size_t i = 0; i < length; ++i)
        if (data[i] >= 'a' && data[i] <= 'z')
            data[i] = static_cast<char>(data[i] - 32);
}

void DynamicString::capitalize() {
    if (length == 0) return;

    // Make whole string lowercase first (to ensure proper capitalization)
    for (size_t i = 0; i < length; ++i)
        if (data[i] >= 'A' && data[i] <= 'Z')
            data[i] = static_cast<char>(data[i] + 32);

    // Capitalize first letter and letters after spaces
    if (data[0] >= 'a' && data[0] <= 'z') data[0] = static_cast<char>(data[0] - 32);

    for (size_t i = 1; i < length; ++i) {
        if (data[i - 1] == ' ' && data[i] >= 'a' && data[i] <= 'z') {
            data[i] = static_cast<char>(data[i] - 32);
        }
    }
}

// Concatenation operators

DynamicString DynamicString::operator+(const DynamicString& other) const {
    DynamicString result;
    delete[] result.data;

    result.length = length + other.length;
    result.data = new char[result.length + 1];

    for (size_t i = 0; i < length; ++i) result.data[i] = data[i];
    for (size_t i = 0; i < other.length; ++i) result.data[length + i] = other.data[i];

    result.data[result.length] = '\0';
    return result;
}

DynamicString& DynamicString::operator+=(const DynamicString& other) {
    char* newData = new char[length + other.length + 1];
    for (size_t i = 0; i < length; ++i) newData[i] = data[i];
    for (size_t i = 0; i < other.length; ++i) newData[length + i] = other.data[i];
    newData[length + other.length] = '\0';

    delete[] data;
    data = newData;
    length += other.length;
    return *this;
}

// Comparison operators

bool DynamicString::operator==(const DynamicString& other) const {
    if (length != other.length) return false;
    for (size_t i = 0; i < length; ++i)
        if (data[i] != other.data[i]) return false;
    return true;
}

bool DynamicString::operator!=(const DynamicString& other) const {
    return !(*this == other);
}

bool DynamicString::operator<(const DynamicString& other) const {
    size_t minLen = (length < other.length ? length : other.length);
    for (size_t i = 0; i < minLen; ++i) {
        int cmp = charCompare(data[i], other.data[i]);
        if (cmp < 0) return true;
        if (cmp > 0) return false;
    }
    return length < other.length;
}

bool DynamicString::operator>(const DynamicString& other) const {
    return other < *this;
}

// Indexing

char& DynamicString::operator[](size_t index) {
    return data[index];
}

const char& DynamicString::operator[](size_t index) const {
    return data[index];
}

// Unary operators

DynamicString DynamicString::operator+() const {
    return DynamicString(*this);
}

DynamicString DynamicString::operator-() const {
    DynamicString temp(*this);
    temp.toLower();
    return temp;
}

// Streams

std::ostream& operator<<(std::ostream& out, const DynamicString& s) {
    out << s.data;
    return out;
}

std::istream& operator>>(std::istream& in, DynamicString& s) {
    // read a whitespace-separated token
    char buffer[1024];
    in >> buffer;

    size_t len = 0;
    while (buffer[len] != '\0') ++len;

    delete[] s.data;
    s.copyFrom(buffer, len);
    return in;
}
