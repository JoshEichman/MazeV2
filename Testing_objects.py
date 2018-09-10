# -*- coding: utf-8 -*-
"""
Created on Mon Sep  3 02:31:20 2018

@author: Josh
"""

class Car(object):
	"""
		blueprint for car
	"""

	def __init__(self, model, color, company, speed_limit):
		self.color = color
		self.company = company
		self.speed_limit = speed_limit
		self.model = model

	def start(self):
		print("started")

	def stop(self):
		print("stopped")

	def accelarate(self):
		print("accelarating...")
		"accelarator functionality here"

	def change_gear(self, gear_type):
		print("gear changed")
		" gear related functionality here"
        

x = Car('model1','color1','company1',100)
x.start()
x.stop()