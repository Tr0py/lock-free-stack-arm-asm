#!/use/bin/env python3
import re

print("Enter log name:")
#logFN = input()
g_ABA = 0
four_threads_pos = 0
start_thread_count = 3
def ana(a_tid, logFN):
    a_tid = str(a_tid)
    
    falseABA = 0
    trueABA = 0
    out = open('errors.txt', 'w+')
    pos = 0
    with open(logFN, 'r') as fLog:
        LL = False
        llsc_addr = ''
        err = 0
        tmp = ''
        for line in fLog:
            pos += 1
            if (pos < four_threads_pos):
                continue
            #print(line)
            
            rObj = re.search(r'thread (.*)!(.*)addr (.*)', line)
            if rObj:
                tid, op, status = rObj.group(1).split() 
                addr = rObj.group(3)
                #print(a_tid, tid, op, status, addr)
                if tid == a_tid:
                    if op == 'ldrex':
                        LL = True
                        err = 0
                        llsc_addr = addr
                        
                        tmp = line
                        #print('begin!')
                    elif op == 'strex':
                        LL = False
                        if status == 'suc':
                            if err == 0:
                                continue
                            tmp += line
                            tmp += '-------%d errors--------\n' % (err)
                            if err == 1:
                                falseABA += 1
                                #print(tmp)
                            elif err > 1:
                                trueABA += 1
                            out.write(tmp)


                    else:
                        print("error op type %s" % (op))
                        exit(1)
                else:
                    if LL == True and status == 'suc' and addr == llsc_addr:
                        err += 1
                        tmp += line
                #if LL == True:
                #    print(tid,op,status)
    out.close()
    return [falseABA, trueABA]

def search_tids(logFN):
    with open(logFN) as fLog:
        tids = set()
        pos = 0
        for line in fLog:
            pos += 1
            rObj = re.search(r'thread (.*)!', line)
            if rObj:
                tid, op, status = rObj.group(1).split() 
                tids.add(tid)
                if (len(tids) == start_thread_count):
                    global four_threads_pos
                    four_threads_pos = pos
                if status == 'suc' and (len(tids) >= start_thread_count):
                    global g_ABA
                    g_ABA += 1
    print('start valve:',four_threads_pos)
    return tids


if __name__=='__main__':
    logFN = 'log'
    lastLL={}
    falseABA = 0
    trueABA = 0
    info = []

    tids = search_tids(logFN)
    print('Found %d threads: %s' % (len(tids), tids))
    for i in tids:
        i = int(i)
        print('[+] analyzing thread %d' % (i))
        fABA, tABA = ana(i,logFN)
        print('falseABA %d, trueABA %d' % (fABA, tABA))
        falseABA += fABA
        trueABA += tABA
        info.append([i,fABA,tABA])
    total = falseABA+trueABA
    print('falseABA %d, trueABA %d, total %d' % (falseABA, trueABA, total))
    print('total LLSC = %d, ABA LLSC = %d, ABA rate = %f' % (g_ABA, total, total/g_ABA))
    #print(info)
