# -*- coding:utf-8 -*-
import Levenshtein
'''
texta = '艾伦 图灵传'
textb = '艾伦和图灵传'
texta_u_ori = u'艾伦 图灵传'
textb_u_ori = u'艾伦和图灵传'
'''

texta = '逸动这款车上市巳经有一段时间了z作为长安旗下明星车型z厂家最近为它进行了动力系统升级。这次推出的逸动蓝动版车型使用了一台功率更犬的启动/发电/驱动一体式电机来取代逸动常规动力车型上的启动电机z可在一定程度上提升它的动力性能并降低油耗水平。'
textb = '逸动这款车上市已经有一段时间了，作为长安旗下明星车型，厂家最近为它进行了动力系统升级。这次推出的逸动蓝动版车型使用了一台功率更大的启动/发电/驱动一体式电机来取代逸动常规动力车型上的启动电机，可在一定程度上提升它的动力性能并降代油耗水平。'

texta_u = texta.decode('utf-8')
textb_u = textb.decode('utf-8')

print type(texta), type(texta_u)
print Levenshtein.distance(texta,textb)
print Levenshtein.distance(texta_u,textb_u)