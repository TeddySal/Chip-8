import re

testo = 'DRW V0,V1,  5'
pulito = re.sub(r'\s+|,', ' ', testo)
print(pulito)
pulito = re.sub(r' +', ' ', pulito)
print(pulito)