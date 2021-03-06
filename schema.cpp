#include "schema.h"

void Row::serialize_row(void *destination)
{
    memcpy(destination + ID_OFFSET, &(this->id), ID_SIZE);
    strncpy((char*)destination + USERNAME_OFFSET, this->username, USERNAME_SIZE);
    strncpy((char*)destination + EMAIL_OFFSET, this->email, EMAIL_SIZE);
}

void Row::deserialize_row(void *source)
{
    memcpy(&(this->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(this->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(this->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void Row::print()
{
    // std::cout << this->id << "\t" << this->username << "\t\t" << this->email << "\n";
    std::cout <<"("<< this->id << ", " << this->username << ", " << this->email << ")\n";

}