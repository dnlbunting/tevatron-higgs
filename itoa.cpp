
TString Itoa(Int_t value, Int_t base)
{
   // Converts an Int_t to a TString with respect to the base specified (2-36).
   // Thus it is an enhanced version of sprintf (adapted from versions 0.4 of
   // http://www.jb.man.ac.uk/~slowe/cpp/itoa.html).
   // Usage: the following statement produce the same output, namely "1111"
   //   std::cout << TString::Itoa(15,2) ;
   //   std::cout << TString::Itoa(0xF,2) ; /// 0x prefix to handle hex
   //   std::cout << TString::Itoa(017,2) ; /// 0  prefix to handle oct
   // In case of error returns the "!" string.

   std::string buf;
   // check that the base if valid
   if (base < 2 || base > 36) {
      Error("TString::Itoa", "base %d is not supported. Supported bases are {2,3,...,36}.",base) ;
      return (TString("!"));
   }
   buf.reserve(35); // Pre-allocate enough space (35=kMaxDigits)
   Int_t quotient = value;
   // Translating number to string with base:
   do {
      buf += "0123456789abcdefghijklmnopqrstuvwxyz"[ TMath::Abs(quotient % base) ];
      quotient /= base;
   } while (quotient);
   // Append the negative sign
   if (value < 0) buf += '-';
   std::reverse(buf.begin(), buf.end());
   return (TString(buf.data()));
}
