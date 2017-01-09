#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
usage: python getEditDis.py -file_recog ./pc_2_2.txt -file_truth ./pc_2_2.box -mode 2
'''

import logging
import time
import argparse
import editdistance #github
import Levenshtein #库函数


logtime = time.strftime('%m-%d-%H-%M', time.localtime(time.time()))
logname = "getEditDis_" + logtime + ".log"

logging.basicConfig(level=logging.DEBUG,
			format='%(asctime)s %(levelname)s : %(message)s',
			datefmt='%Y-%b-%d %H:%M:%S',
			filename=logname,
			filemode='w')

def getTxt_truth(file_name):
    str_rst = ''
    with open(file_name, 'r') as fileTxt:
        for line in fileTxt.readlines():
            line = line.strip( )
            line = line.split(' ')
            str_rst += line[0]
    return str_rst

def getTxt_recog(file_name):
    str_rst = ''
    bFindRst = False
    with open(file_name, 'r') as fileTxt:
        for line in fileTxt.readlines():
            line = line.strip()
            line = line.split(' ')

            if line[0] == 'recognition':
                bFindRst = True
                line = line[2:]
            if not bFindRst:
                continue
            length = len(line)
            for idx in range(0, length):
                str_rst += line[idx]
    return str_rst

def parse_args():
    parser = argparse.ArgumentParser(description='get Levenshtein Distance')
    parser.add_argument('-file_recog', dest='txt_file1', help='rst of recog',
                        default='./recog.txt')
    parser.add_argument('-file_truth', dest='txt_file2', help='truth content',
                        default='./truth.txt')
    parser.add_argument('-mode', dest='mode',type=int, help='1:editdistance, 2:Levenshtein',
                        default=1)
    parser.add_argument('-output_dir', dest='output_dir', help='rst directory',
                        default='./')
    args = parser.parse_args()
    return args


if __name__ == '__main__':
    args = parse_args()

    logging.info("-file_recog: %s" % (args.txt_file1))
    logging.info("-file_truth: %s" % (args.txt_file2))
    logging.info("-mode: %s" % (args.mode))
    logging.info("-output_dir: %s" % (args.output_dir))

    str_recog = getTxt_recog(args.txt_file1)
    str_truth = getTxt_truth(args.txt_file2)
    str_recog_u = str_recog.decode('utf-8')
    str_truth_u = str_truth.decode('utf-8')
    if args.mode == 1:
        dis_str = editdistance.eval(str_truth, str_recog)
        dis_str_u = editdistance.eval(str_truth_u, str_recog_u)
    elif args.mode ==2:
        dis_str = Levenshtein.distance(str_truth, str_recog)
        dis_str_u = Levenshtein.distance(str_truth_u, str_recog_u)

    print "dis_str :", dis_str
    print "dis_str_u :", dis_str_u
    logging.info('recog %s'%(str_recog))
    logging.info('truth %s' %(str_truth))
    logging.info('dis is %d, total len %d'%(dis_str_u, len(str_truth_u)))

