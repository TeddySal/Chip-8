import re

def resolve_address(address, args=[], n=0):
    if n == 0:
        return address
    elif n == 1:
        return address | args[0]
    elif n == 2:
        return address | (args[0] << 8) | args[1]
    elif n == 3:
        return address | (args[0] << 8) | (args[1] << 4) | args[2]
    else:
        assert False, "Not permitted"

def resolve_ld(args):
    if args[0] == 'I':
        return 0xA000 | args[1]
    else:
        return 0x6000 | (args[0] << 8) | args[1]

INSTRUCTIONS = {
    'CLS': lambda args: resolve_address(0x00E0),
    'RET': lambda args: resolve_address(0x00EE),
    'JP' : lambda args: resolve_address(0x1000, args, 1),
    'LD' : lambda args: resolve_ld(args),
    'DRW': lambda args: resolve_address(0xD000, args, 3),
    'ADD': lambda args: resolve_address(0x7000, args, 2),
    'DB' : lambda args: args
}
labels = {}
opcodes = []

def get_content_file(file_path: str) -> str :
    with open(file_path, 'r') as f:
        return f.read()
    
def is_data(com) -> bool:
    if is_label(com):
        if com.split()[1] == 'db':
            return True
        else:
            return False
    else:
        if com.split()[0] == 'db':
            return True
        else:
            return False

def is_label(com: str) -> bool:
    if com != '' and com.split()[0][-1] == ':':
        return True
    else:
        return False

def adjust_parts(com: str) -> str:
    pulito = re.sub(r'\s+|,', ' ', com)
    return re.sub(r' +', ' ', pulito)

def parse_asm(content):
    start_addres = 0x200
    for line in content:
        com = line.strip().split(';')[0]
        if not com:
            continue 
        if is_label(com):
            com = com.split(":", 1)[1].strip()

        parts = adjust_parts(com).split()#com.replace(',', '').split()
        mnemonic = parts[0].upper()
        if mnemonic not in INSTRUCTIONS:
            raise ValueError(f'Unknown instruction {mnemonic}')
        args_raw = parts[1:]
        args = []
        #print(args_raw)
        for arg in args_raw:
            if arg.startswith('V'):
                args.append(int(arg[1:], 16))
            elif arg.startswith('#'):
                args.append(int(arg[1:], 16))
            elif arg in labels:
                args.append(labels[arg])
            else:
                args.append(int(arg) if arg.isdigit() else arg)

        opcode = INSTRUCTIONS[mnemonic](args)
        opcodes.append(opcode)
        start_addres += 2   

def set_labels(content):
    start_addres = 0x200
    for line in content:
        com = adjust_parts(line)
        if not com:
            continue
        elif is_label(com):
            label_name = com.split()[0][:-1]
            labels[label_name] = start_addres
            if is_data(com):    
                nums = len(com.split()[2:])
                #print(f'nums: {nums}')
                start_addres += nums
            else:
                start_addres += 2
        elif is_data(com):
            nums = len(com.split()[1:])
            start_addres += nums
        else:
            start_addres += 2

def generate_ch8():
    with open('out.ch8', 'wb') as out:
        for op in opcodes:
            if type(op) != list:
                out.write(op.to_bytes(2, 'big'))
            else:
                for x in op:
                    out.write(x.to_bytes(1, 'big'))

def main() -> None:
    file_path = 'test_2.asm'
    content = get_content_file(file_path).splitlines()

    set_labels(content)

    parse_asm(content)
    #print(labels)
    generate_ch8()

if __name__ == '__main__':
    main()