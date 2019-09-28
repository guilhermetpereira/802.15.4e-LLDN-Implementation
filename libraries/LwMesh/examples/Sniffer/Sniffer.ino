#ifdef __cplusplus
extern "C" {
#endif

void mac_init(void);
void mac_task(void);

#ifdef __cplusplus
}
#endif

void setup()
{
  mac_init();
}

void loop()
{
  mac_task();
}
