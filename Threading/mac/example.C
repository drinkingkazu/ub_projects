{
  //
  // I do not recommend using CINT.
  // But that's your decision :)
  //

  // A simple CINT macro to show how "foo" class under "simple"
  // package can be used in CINT interpreter.

  // Step 1: Load foo library
  gSystem->Load("libfoo");

  // Step 2: Play
  foo myfoo;
  myfoo.speak();

}
