#include "unity.h"
#include "clock.h"
/*
1) Configurar la biblioteca, consultar la hora y tiene que ser invalida.
2) Configurar la biblioteca, ajustar la hora (con valores correctos), consultar la hora y tiene que ser valida.
3) Configurar la biblioteca, ajustar la hora (con valores incorrectos) tiene devolver un error y al consultar la hora tiene que ser invalida.
4) Simular el paso de n ciclos de reloj, consultar la hora y verificar que avanzo un segundo
5) Simular el paso de 10*n ciclos de reloj, consultar la hora y verificar que avanzo diez segundos
6) Simular el paso de un minuto, diez minutos, una hora, diez horas y un día completo.
7) Configurar la hora de la alarma (con valores correctos) y revisar si la guarda.
8) Configurar la hora de la alarma (con valores incorrectos) y revisar si la rechaza.
9) Configurar la hora de la alarma (con valores correctos) y revisar si la queda activada.
10) Si la alarma esta activa y la desactivo, queda desactivada, pero no cambia la hora.
11) Si la alarma esta desactivada y la activo, queda activa, pero no cambia la hora.
12) Si la alarma esta activa, y la hora del reloj coincide con la hora de la alarma, entonces suena.
14) Si la se activo, y las pospongo n minutos, vuelve a sonar n minutos
15) Si la se activo, y la cancelo, al día siguiente, vuelve a sonar
16) Si alarma se activa y pasan 23:59:59 no debe volver a activarse, pero si pasa un segundo mas entonces si se debe volver a activar
17) Si la alarma esta inactiva no debe sonar cuando la hora del reloj coincida con la hora de la alarma
*/
#define TICKS_PER_SECOND 5

clock_t reloj;
bool alarm_state;

void SimulateSeconds(int seconds) {
    for(int index = 0; index < seconds * TICKS_PER_SECOND; index ++) {
        ClockNewTicket(reloj);
    }

}

void AlarmEventHandler(clock_t clock,bool state){
    alarm_state = state;
}

void setUp(void){
    static const uint8_t INICIAL[] = {1,2,3,4};
    reloj = ClockCreate(TICKS_PER_SECOND, AlarmEventHandler);
    ClockSetupTime(reloj,INICIAL, sizeof(INICIAL));
    alarm_state = false; 
}

void test_sart_up(void) {
    //static const uint8_t INICIAL[] = {1,2,3,4};
    static const uint8_t ESPERADO[] = {0,0,0,0,0,0};
    uint8_t hora[6];
    clock_t reloj = ClockCreate(TICKS_PER_SECOND, AlarmEventHandler);
    //ClockSetupTime(reloj,INICIAL, sizeof(INICIAL));
    TEST_ASSERT_FALSE(ClockGetTime(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora,sizeof(ESPERADO));
    TEST_ASSERT_FALSE(ClockGetAlarm(reloj, hora, sizeof(hora)));
}

void test_set_up_curret_time(void){
    static const uint8_t INICIAL[] = {1,2,3,4};
    static const uint8_t ESPERADO[] = {1,2,3,4,0,0};
    uint8_t hora[6];
    clock_t reloj = ClockCreate(TICKS_PER_SECOND, AlarmEventHandler);
    ClockSetupTime(reloj, INICIAL, sizeof(INICIAL));
    TEST_ASSERT_TRUE(ClockGetTime(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora,sizeof(ESPERADO));

}

void test_one_second_elapsed(void){
    //static const uint8_t INICIAL[] = {1,2,3,4};
    static const uint8_t ESPERADO[] = {1,2,3,4,0,1};
    uint8_t hora[6];

    //clock_t reloj = ClockCreate(TICKS_PER_SECOND);
    //ClockSetupTime(reloj,INICIAL, sizeof(INICIAL));
    //for(int index = 0; index < TICKS_PER_SECOND; index ++) {
    //    ClockNewTicket(reloj);
    //}
    SimulateSeconds(1);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora,sizeof(ESPERADO));
}

void test_ten_second_elapsed(void){
    
    static const uint8_t ESPERADO[] = {1,2,3,4,1,0};
    uint8_t hora[6];
    SimulateSeconds(10);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora,sizeof(ESPERADO));
}

void test_one_minute_elapsed(void){
    
    static const uint8_t ESPERADO[] = {1,2,3,5,0,0};
    uint8_t hora[6];
    SimulateSeconds(60);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora,sizeof(ESPERADO));
}
void test_ten_minute_elapsed(void){
    
    static const uint8_t ESPERADO[] = {1,2,4,4,0,0};
    uint8_t hora[6];
    SimulateSeconds(10 * 60);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora,sizeof(ESPERADO));
}

void test_setup_and_get_alarm(void) {
    static const uint8_t ALARMA[] = {1,2,3,5};
    uint8_t hora[4];

    ClockSetupAlarm(reloj,ALARMA, sizeof(ALARMA));
    TEST_ASSERT_TRUE(ClockGetAlarm(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARMA, hora, sizeof(ALARMA));
}

void test_setup_and_disable_alarm(void) {
    static const uint8_t ALARMA[] = {1,2,3,5};
    uint8_t hora[4];

    ClockSetupAlarm(reloj,ALARMA, sizeof(ALARMA));
    TEST_ASSERT_FALSE(ClockToggleAlarm(reloj));
    TEST_ASSERT_FALSE(ClockGetAlarm(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARMA, hora, sizeof(ALARMA));
}

void test_setup_and_fire_alarm(void) {
    static const uint8_t ALARMA[] = {1,2,3,5};
    
    ClockSetupAlarm(reloj,ALARMA, sizeof(ALARMA));
    SimulateSeconds(60);
    TEST_ASSERT_TRUE(alarm_state);
 //   alarm_state = false;
 //   SimulateSeconds(10);
 //   TEST_ASSERT_FALSE(alarm_state);
    

}

void test_setup_and_not_fire_alarm(void) {
    static const uint8_t ALARMA[] = {1,2,3,5};
    
    ClockSetupAlarm(reloj,ALARMA, sizeof(ALARMA));
    ClockToggleAlarm(reloj);
    SimulateSeconds(60);
    TEST_ASSERT_FALSE(alarm_state);

}