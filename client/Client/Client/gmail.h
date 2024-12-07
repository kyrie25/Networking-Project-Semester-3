#pragma once
#include <string>

//Token
std::string getAccessToken();

//ReadGmail
std::string getEmail(const std::string& accessToken, const std::string& messageId);
std::string getMessageId(const std::string& accessToken);

bool isAdmin(const std::string& accessToken, const std::string& senderEmail, const std::string& messageId);

//SendGmail
void sendEmail(const std::string& accessToken, const std::string& to, const std::string& subject, const std::string& body);
void sendGmailWithAttachment(const std::string& accessToken, const std::string& to, const std::string& subject, const std::string& body, const std::string& attachmentPath);