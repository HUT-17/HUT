@echo off
set target=mv_digicap.dav
echo ��������֧��Gige���豸IP��
gigetool -l
set /p ip=������Ҫ�������豸ip: 
hik_upg_cli %ip% %target%
echo ���س��˳�����
pause>nul