/** @babel */

import _ from 'lodash'

export default class Logger {
  constructor () {
    this.messages = []
    this._group = null
  }

  error (text, filePath, range, logPath, logRange) {
    this.showMessage({ type: 'Error', text: text, filePath: filePath, range: range, logPath: logPath, logRange: logRange })
  }

  warning (text, filePath, range, logPath, logRange) {
    this.showMessage({ type: 'Warning', text: text, filePath: filePath, range: range, logPath: logPath, logRange: logRange })
  }

  info (text, filePath, range, logPath, logRange) {
    this.showMessage({ type: 'Info', text: text, filePath: filePath, range: range, logPath: logPath, logRange: logRange })
  }

  showMessage (message) {
    message = _.pickBy(message)
    if (this._group) {
      this._group.push(message)
    } else {
      this._label = 'LaTeX Message'
      this._group = [message]
      this.groupEnd()
    }
  }

  group (label) {
    this._label = label
    this._group = []
  }

  groupEnd () {
    this.messages = _.sortBy(this._group, 'filePath', message => { return message.range || [[-1, -1], [-1, -1]] }, 'type', 'text')
    this._group = null
    this.showFilteredMessages()
  }

  showFilteredMessages () {
    const loggingLevel = atom.config.get('latex.loggingLevel')
    const showBuildWarning = loggingLevel !== 'error'
    const showBuildInfo = loggingLevel === 'info'
    const filteredMessages = _.filter(this.messages, (message) => {
      return message.type === 'Error' || (showBuildWarning && message.type === 'Warning') || (showBuildInfo && message.type === 'Info')
    })

    this.showMessages(this._label, filteredMessages)
  }

  static getMostSevereType (messages) {
    return _.reduce(messages, (type, message) => {
      if (type === 'Error' || message.type === 'Error') return 'Error'
      if (type === 'Warning' || message.type === 'Warning') return 'Warning'
      return type
    }, undefined)
  }

  showMessages (/* label, messages */) {}
  sync () {}
  toggle () {}
  show () {}
  hide () {}

}
